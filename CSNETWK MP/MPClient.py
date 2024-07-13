import socket
import threading
SERVER_IP = "127.0.0.1"
SERVER_PORT = 12345
BUFFER_SIZE = 1024

joined = False  
is_registered = False

def receive_data(socket):
    global joined,is_registered

    while True:
        try:
            data = socket.recv(BUFFER_SIZE).decode("utf-8")
            print(data)

            if data.startswith("/get"):
                
                parts = data.split(" ", 3)  
                filename = parts[1]
                file_size = int(parts[2])
                timestamp = parts[3]

                
                file_content = socket.recv(file_size).decode("utf-8")

                
                with open(filename, "w") as file:
                    file.write(file_content)
                print(f"File has been received from Server: {filename} ({timestamp})")

            
            elif "Connection to the File Exchange Server is successful!" in data:
                joined = True
                is_registered = True
                print("Client has joined:", is_registered)
        except Exception as e:
            print(f"Error receiving data: {str(e)}")
            break



def send_command(socket):
    global joined, is_registered

    while True:
        command = input("Please enter a command: ")

        if not joined:
            if "/register" in command:
                print("Error: Please join to the server before registering.")
            elif "/leave" in command:
                print("Error: Please join to the server before leaving.")
            elif "/dir" in command or "/store" in command or "/get" in command:
                print("Error: You need to join and register before using this command.")
            else:
                socket.send(command.encode("utf-8"))
        else:
            if "/dir" in command or "/store" in command or "/get" in command:
                if not is_registered:
                    print("Error: You need to register before using this command.")
                else:
                    socket.send(command.encode("utf-8"))
            else:
                socket.send(command.encode("utf-8"))


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((SERVER_IP, SERVER_PORT))

    threading.Thread(target=receive_data, args=(client,), daemon=True).start()
    threading.Thread(target=send_command, args=(client,), daemon=True).start()

    
    while True:
        pass

if __name__ == "__main__":
    main()

