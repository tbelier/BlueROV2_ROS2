from moviepy.editor import VideoFileClip
# Charger l'image .webp
input_file = "/media/tbelier/usbKey/Robotics/Videos/Roll/RollVuePC.webm"  # Remplacez par le nom de votre fichier .webp
output_file = "/media/tbelier/usbKey/Robotics/Videos/Roll/RollVuePC.mp4"  # Nom du fichier de sortie



# Charger le fichier WebM
clip = VideoFileClip(input_file)

# Convertir et enregistrer en MP4
clip.write_videofile(output_file, codec="libx264")

