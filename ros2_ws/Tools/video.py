from flask import Flask, Response, render_template_string
import cv2
import threading
from ultralytics import YOLO
import cv2

# Charger le modèle YOLO
model = YOLO("yolo11s.pt")
app = Flask(__name__)

# Ouvrir la caméra
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
if not cap.isOpened():
    raise Exception("Impossible d'ouvrir la caméra")

# Variables pour le buffering
frame_lock = threading.Lock()  # Verrou pour protéger l'accès au buffer
current_frame = None           # Dernière frame capturée

# Thread pour capturer les frames en continu
def capture_frames():
    global current_frame
    while True:
        success, frame = cap.read()
        if success:
            encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 50]
            results = model.predict(source=frame, conf=0.4, show=False)  # Désactive `show` pour ne pas ouvrir une fenêtre
            annotated_frame = results[0].plot()
            _, buffer = cv2.imencode('.jpg', annotated_frame, encode_param)
            with frame_lock:
                current_frame = buffer.tobytes()

# Lancer le thread de capture
capture_thread = threading.Thread(target=capture_frames, daemon=True)
capture_thread.start()

# Gérer le flux vidéo pour chaque utilisateur
import time

# Gérer le flux vidéo avec un délai pour augmenter les FPS
def generate_frames():
    while True:
        with frame_lock:
            if current_frame is None:
                continue
            frame = current_frame
        # Attendre 1/30e de seconde (pour ~30 FPS)
        time.sleep(1/30)
        # Envoyer la réponse en streaming
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


# Route principale pour afficher la page HTML
@app.route('/')
def index():
    return render_template_string('''
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>En direct</title>
        </head>
        <body>
            <h1>Coucou choupi</h1>
            <img src="{{ url_for('video_feed') }}" alt="Flux vidéo" style="max-width: 100%; height: auto;">
        </body>
        </html>
    ''')

# Route pour diffuser le flux vidéo
@app.route('/video_feed')
def video_feed():
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=60000)
