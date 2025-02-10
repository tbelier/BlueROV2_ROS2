import numpy as np
import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

# Fonction pour lire un CSV et retourner un numpy array de floats
def csv_to_numpy(filename):
    data = []
    
    with open(filename, mode='r', newline='', encoding='utf-8') as file:
        reader = csv.reader(file)
        next(reader)  # Ignorer les titres
        
        for row in reader:
            try:
                data.append([float(value) if value.strip() else np.nan for value in row])  # Convertir en float, remplacer les vides par NaN
            except ValueError:
                continue  # Ignorer les lignes corrompues
    
    return np.array(data)

import numpy as np

def calculate_position(range, elevation, azimuth, yaw, pitch, roll):
    """
    Calcule les coordonnées locales (x, y, z) de la balise à partir des données USBL et IMU.

    Arguments :
    - range : Distance entre le robot et la balise (m).
    - elevation : Angle d'élévation (radians).
    - azimuth : Angle d'azimut (radians).
    - yaw : Angle de lacet (radians).
    - pitch : Angle de tangage (radians).
    - roll : Angle de roulis (radians).

    Retour :
    - x, y, z : Coordonnées dans le repère global.
    """
    # Étape 1 : Conversion des angles USBL en coordonnées locales
    x_local = range * np.cos(elevation) * np.cos(azimuth)
    y_local = range * np.cos(elevation) * np.sin(azimuth)
    z_local = range * np.sin(elevation)

    # Étape 2 : Matrice de rotation pour yaw, pitch, roll
    R_yaw = np.array([
        [np.cos(yaw), -np.sin(yaw), 0],
        [np.sin(yaw), np.cos(yaw),  0],
        [0,           0,            1]
    ])

    R_pitch = np.array([
        [np.cos(pitch), 0, np.sin(pitch)],
        [0,             1, 0           ],
        [-np.sin(pitch), 0, np.cos(pitch)]
    ])

    R_roll = np.array([
        [1, 0,           0           ],
        [0, np.cos(roll), -np.sin(roll)],
        [0, np.sin(roll), np.cos(roll)]
    ])

    # Matrice de rotation finale
    R = R_yaw @ R_pitch @ R_roll  # Produit matriciel : ordre Yaw -> Pitch -> Roll

    # Étape 3 : Transformation des coordonnées locales vers le repère global
    position_local = np.array([x_local, y_local, z_local])
    position_global = R @ position_local

    return position_global[0], position_global[1], position_global[2]

"""
# Exemple d'utilisation
if __name__ == "__main__":
    # Données USBL et IMU (exemple)
    range = 10.0           # Distance en mètres
    elevation = np.radians(30)  # Angle d'élévation (converti en radians)
    azimuth = np.radians(45)    # Angle d'azimut (converti en radians)
    yaw = np.radians(10)        # Yaw (en radians)
    pitch = np.radians(5)       # Pitch (en radians)
    roll = np.radians(2)        # Roll (en radians)
"""



# Chargement des données
filename = 'ros2_ws/250205_rosbagStabilisationProfondeurClean/save.csv'  
tableau_numpy = csv_to_numpy(filename)

# Extraction des colonnes X, Y, Z (indices 36, 37, 38)
x, y, z = [], [], []

for k in range(len(tableau_numpy)):
    try:
        if not np.isnan(tableau_numpy[k, 36]):  # Vérifier si la valeur est valide
            x.append(tableau_numpy[k, 43])
            y.append(-tableau_numpy[k, 44])
            z.append(tableau_numpy[k, 45])
    except IndexError:
        continue  # Si la ligne ne contient pas assez de colonnes
x, y, z = calculate_position(range, elevation, azimuth, yaw, pitch, roll)
print(f"Position locale (x, y, z) : ({x:.2f}, {y:.2f}, {z:.2f})")

# Création d'une échelle de couleurs basée sur le temps (indices des points)
time = np.linspace(0, 1, len(x))  # Normaliser les indices entre 0 et 1
colors = cm.coolwarm(time)  # Utiliser une carte de couleurs de bleu à rouge

# Création du plot 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Tracer les points avec des couleurs variables
scatter = ax.scatter(z, x, y, c=time, cmap='coolwarm', s=20)


# Ajout de labels
ax.set_xlabel('Z')
ax.set_ylabel('X')
ax.set_zlabel('Y')
ax.set_title("Déplacement du ROV dans l'espace")

# Affichage
ax.legend()
plt.show()

#__time,/attitude_twist/angular/x,/attitude_twist/angular/y,/attitude_twist/angular/z,/attitude_twist/linear/x,/attitude_twist/linear/y,/attitude_twist/linear/z,/joy/axes[0],/joy/axes[1],/joy/axes[2],/joy/axes[3],/joy/axes[4],/joy/axes[5],/joy/axes[6],/joy/axes[7],/joy/buttons[0],/joy/buttons[10],/joy/buttons[11],/joy/buttons[1],/joy/buttons[2],/joy/buttons[3],/joy/buttons[4],/joy/buttons[5],/joy/buttons[6],/joy/buttons[7],/joy/buttons[8],/joy/buttons[9],/joy/header/stamp/nanosec,/joy/header/stamp/sec,/real/u/angular/x,/real/u/angular/y,/real/u/angular/z,/real/u/linear/x,/real/u/linear/y,/real/u/linear/z,/sensor/pressure/data,/usbl_data/azimuth,/usbl_data/beacon_id,/usbl_data/depth,/usbl_data/elevation,/usbl_data/header/stamp/nanosec,/usbl_data/header/stamp/sec,/usbl_data/pitch,/usbl_data/position/x,/usbl_data/position/y,/usbl_data/position/z,/usbl_data/range,/usbl_data/roll,/usbl_data/yaw
#   1       2                                   3                   4                               5                   6                           7               8               9           10              11          12         13           14          15          16                  17              18                  19              20              21              22              23              24          25              26                  27                28                    29                      30                  31             32               33                  34              35                    35                 36              37                   38                     39           40                                    41                          42                   43             44                      45                   46                  47       48                