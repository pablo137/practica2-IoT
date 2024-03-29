import socket
import os
from _thread import *
import time

ServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = '192.168.0.14' #direccion ipv4 Lan inalambrica
port = 5000

reply = "NOT DEFINED"

try:
    ServerSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Waiting for a Connection...')
ServerSocket.listen(1)

print(f"[*] Escuchando en {host}:{port}")

def threaded_client(connection, client_address):
    global reply
    while True:
        # Recibe los datos del cliente
        data = connection.recv(1024).decode('utf-8')
        #distance = int(data)   
        # Verificar si la cadena es convertible a un entero
        if data.strip():  # Verifica si la cadena no está vacía después de eliminar los espacios en blanco
            try:
                distance = int(data)
                print("Número recibido:", distance)
            except ValueError:
                print("La cadena no es convertible a un entero válido:", data)
                distance = -1  # Asignar un valor predeterminado en caso de error
        else:
             distance = -1  # Asignar un valor predeterminado si la cadena está vacía

        # Actualiza la variable 'reply' según la distancia
        if distance < 0:
            reply = "NOT DEFINED"
        elif distance < 30:
            reply = "BLUE"
        elif 30 <= distance < 60:
            reply = "ORANGE"
        elif 60 <= distance < 120:
            reply = "RED"

        # Envía 'reply' al cliente
        try:
            connection.sendall(reply.encode('utf-8'))
        except ConnectionAbortedError as e:
            print("Error al enviar respuesta al cliente:", e)

        time.sleep(1)

    connection.close()

with ServerSocket:
    while True:
        client_socket, client_address = ServerSocket.accept()
        print(f"[*] Conexión establecida desde: {client_address[0]}:{client_address[1]}")
        threaded_client(client_socket, client_address)

ServerSocket.close()