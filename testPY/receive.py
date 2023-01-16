import io
import serial

ser = serial.Serial("/dev/ttyS0", 115200)

# Création d'un flux de fichiers pour stocker les données reçues
file_stream = io.BytesIO()

# Lecture des données en provenance du port série
file_stream.write(ser.read())

# Enregistrement des données dans un fichier
with open("received_file.txt", "wb") as f:
    f.write(file_stream.getvalue())