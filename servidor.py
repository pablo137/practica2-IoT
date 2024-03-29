import socket
import threading

# Función para manejar la conexión con cada cliente
def handle_client(client_socket, client_address):
    print(f"[*] Conexión establecida desde: {client_address[0]}:{client_address[1]}")

    try:
        while True:
            # Recibir datos del cliente
            data = client_socket.recv(1024)
            if not data:
                break
            
            # Imprimir los datos recibidos
            print(f"Datos recibidos desde {client_address[0]}:{client_address[1]}: {data.decode('utf-8')}")

            # Reenviar los datos al otro cliente
            other_client_socket.sendall(data)

    except ConnectionResetError:
        print(f"Conexión con {client_address} cerrada.")
    finally:
        client_socket.close()

# Configuración del servidor
HOST = '192.168.0.7'  # Escuchar en todas las interfaces de red
PORT = 5000

# Crear un socket TCP
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Enlazar el socket al host y puerto especificados
server_socket.bind((HOST, PORT))

# Escuchar conexiones entrantes
server_socket.listen(2)  # Máximo 2 conexiones simultáneas

print(f"[*] Escuchando en {HOST}:{PORT}")

# Esperar y manejar conexiones entrantes
try:
    # Aceptar la primera conexión
    client_socket1, client_address1 = server_socket.accept()
    
    # Imprimir que se ha conectado el primer cliente
    print(f"[*] Primer cliente conectado desde: {client_address1[0]}:{client_address1[1]}")
    
    # Aceptar la segunda conexión
    client_socket2, client_address2 = server_socket.accept()
    
    # Imprimir que se ha conectado el segundo cliente
    print(f"[*] Segundo cliente conectado desde: {client_address2[0]}:{client_address2[1]}")

    # Crear dos hilos para manejar la comunicación con cada cliente
    threading.Thread(target=handle_client, args=(client_socket1, client_address1)).start()
    threading.Thread(target=handle_client, args=(client_socket2, client_address2)).start()

except KeyboardInterrupt:
    print("Servidor apagado manualmente.")
finally:
    server_socket.close()

