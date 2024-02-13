import argparse
import socket

class GDBClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.socket.connect((self.host, self.port))
        print("Connected to GDB server")

    def send_command(self, command):
        self.socket.sendall(command.encode())
        print("Sent command:", command)

    def receive_response(self):
        response = self.socket.recv(1024).decode()
        print("Received response:", response)
        return response

    def close(self):
        self.socket.close()
        print("Connection closed")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="GDB Client Program")
    parser.add_argument("--host", default="localhost", help="GDB server host (default: localhost)")
    parser.add_argument("--port", type=int, default=12345, help="GDB server port (default: 12345)")

    args = parser.parse_args()

    client = GDBClient(args.host, args.port)
    client.connect()

    try:
        while True:
            command = input("Enter GDB command (or 'exit' to quit): ")
            if command.lower() == 'exit':
                break
            client.send_command(command)
            client.receive_response()

    finally:
        client.close()
