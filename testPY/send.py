import io
import serial

ser = serial.Serial("/dev/ttyS0", 115200)

# Ouverture du fichier à envoyer
with open("file.txt", "rb") as f:
    # Lecture du fichier en tant que flux de bytes
    file_stream = io.BytesIO(f.read())

# Envoi du fichier par le port série
ser.write(file_stream.read())