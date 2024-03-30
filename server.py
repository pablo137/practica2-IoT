import socket
import threading

class Server:
    def __init__(self, port):
        self.port = port
        self.distance = 0

    def start(self):
        self.listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.listener.bind(('0.0.0.0', self.port))
        self.listener.listen(5)
        print("Server listening on port", self.port)
        while True:
            client, _ = self.listener.accept()
            client_thread = threading.Thread(target=self.handleClient, args=(client,))
            client_thread.start()

    def getLedDistance(self):
        if self.distance > 0 and self.distance < 3:
            return "1000000"
        elif self.distance >= 3 and self.distance < 12:
            return "0100000"
        elif self.distance >= 12 and self.distance < 21:
            return "0010000"
        elif self.distance >= 21 and self.distance < 35:
            return "0001000"
        elif self.distance >= 35 and self.distance < 47:
            return "0000100"
        elif self.distance >= 47 and self.distance < 53:
            return "0000010"
        elif self.distance >= 53 and self.distance < 62:
            return "0000001"
        else:
            return "0000000"

    def handleClient(self, client):
        with client:
            while True:
                data = client.recv(1024)
                if not data:
                    break
                message = data.decode().strip()
                if message.startswith("SET"):
                    new_distance = int(message[4:])
                    self.distance = new_distance
                    print("CURRENT DISTANCE =", self.distance)
                elif message.startswith("GET"):
                    client.sendall(self.getLedDistance().encode())
                else:
                    print("Command not found!", message)
                

def main():
    server = Server(1234)
    server.start()

if __name__ == "__main__":
    main()
