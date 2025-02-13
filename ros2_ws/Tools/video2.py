from ultralytics import YOLO
import cv2

# Charger le modèle YOLO
model = YOLO("yolo11s.pt")

# Ouvrir le flux vidéo depuis la webcam (ou un fichier vidéo)
cap = cv2.VideoCapture(0)  # Remplacez 0 par le chemin d'une vidéo si nécessaire

# Vérifier si la capture vidéo est ouverte
if not cap.isOpened():
    print("Erreur : Impossible d'ouvrir le flux vidéo.")
    exit()

# Dossier pour sauvegarder les résultats
output_path = "runs/detect/video_output.avi"
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = int(cap.get(cv2.CAP_PROP_FPS))


while True:
    ret, frame = cap.read()
    if not ret:
        print("Fin du flux vidéo ou erreur de lecture.")
        break

    # Faire des prédictions sur chaque frame
    results = model.predict(source=frame, conf=0.4, show=False)  # Désactive `show` pour ne pas ouvrir une fenêtre

    # Récupérer l'image annotée par YOLO
    annotated_frame = results[0].plot()

    # Afficher la frame annotée dans une fenêtre OpenCV
    cv2.imshow("YOLO Detection", annotated_frame)


    # Quitter si l'utilisateur appuie sur 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Libérer les ressources
cap.release()
cv2.destroyAllWindows()

print(f"Vidéo avec détections sauvegardée sous : {output_path}")
