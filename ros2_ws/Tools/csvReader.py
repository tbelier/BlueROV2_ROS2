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

# Chargement des données
filename = "/home/tbelier/Documents/GIT/BlueROV2_ROS2/ros2_ws/rosbag2_2025_02_13-14_39_38/logs.csv"
tableau_numpy = csv_to_numpy(filename)

# Extraction des colonnes X, Y, Z (indices 36, 37, 38)
x, y, z = [], [], []

for k in range(len(tableau_numpy)):
    try:
        if not np.isnan(tableau_numpy[k, 33]):  # Vérifier si la valeur est valide
            x.append(tableau_numpy[k, 31])
            y.append(-tableau_numpy[k, 32])
            z.append(tableau_numpy[k, 33])
    except IndexError:
        continue  # Si la ligne ne contient pas assez de colonnes

x, y, z = np.array(x), np.array(y), np.array(z)

# Création d'une échelle de couleurs basée sur le temps (indices des points)
time = np.linspace(0, 1, len(x))  # Normaliser les indices entre 0 et 1
colors = cm.coolwarm(time)  # Utiliser une carte de couleurs de bleu à rouge

# Création du plot 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Tracer les points avec des couleurs variables
ax.plot(x,y,-z)
scatter = ax.scatter(x,y,-z, c=time, cmap='coolwarm', s=20)


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