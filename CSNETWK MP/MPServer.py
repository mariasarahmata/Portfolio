import socket
import threading
import os
from datetime import datetime

SERVER_IP = "127.0.0.1"
SERVER_PORT = 12345
BUFFER_SIZE = 1024

registered_clients = {}

def handle_client(client_socket, client_address, client_handle):

    try:
        while True:
            data = client_socket.recv(BUFFER_SIZE).decode("utf-8")
            if not data:
                break

            process_command(client_socket, client_handle, data)

    except socket.error as e:
        print(f"Socket error handling client {client_handle}: {str(e)}")
    
    except ConnectionResetError:
        print(f"ConnectionResetError handling client {client_handle}")

    except Exception as e:
     print(f"Error handling client {client_handle}: {str(e)}")

    finally:
     remove_client(client_handle)
     client_socket.close()


def process_command(client_socket, client_handle, command):
    
    command_parts = command.split()
    keyword = command_parts[0]

    if keyword == "/join":
        if len(command_parts) == 3:
            join_server(client_socket, client_handle, command_parts[1], int(command_parts[2]))
        else:
            send_error(client_socket, "Invalid /join command syntax")

    elif keyword == "/leave":
        remove_client(client_handle)
        send_message(client_socket, "Connection closed. Thank you!")

    elif keyword == "/register":
        if len(command_parts) == 2:
            register_client(client_socket, client_handle, command_parts[1])
        else:
            send_error(client_socket, "Invalid /register command syntax")

    elif keyword == "/store":
        if not is_registered:
            send_error(client_socket, "Error: You must register first.")
            return
        
        if len(command_parts) == 2:
            store_file(client_socket, client_handle, command_parts[1])
        else:
            send_error(client_socket, "Invalid /store command syntax")

    elif keyword == "/dir":

        send_directory_list(client_socket, client_handle)

    elif keyword == "/get":
        if not is_registered:
            send_error(client_socket, "Error: You must register first.")
            return
        
        if len(command_parts) == 2:
            filename = command_parts[1]
            print(f"Server received /get command for file: {filename}")
            send_file(client_socket, client_handle, filename)
        else:
            send_error(client_socket, "Invalid /get command syntax")

    elif keyword == "/?":
        send_help(client_socket)

    else:
        send_error(client_socket, "Sorry, Command not found")

def join_server(client_socket, client_handle, server_ip, server_port):
    global is_registered  

    if server_ip != "127.0.0.1":
        send_error(client_socket, "Error: Invalid IP address for join command")
        return

    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.connect((server_ip, server_port))

        registered_clients[client_handle] = {"socket": server_socket}
        send_message(client_socket, "Connection to the File Exchange Server is successful!")

        threading.Thread(target=handle_client, args=(server_socket, None, f"Server-{client_handle}")).start() 

    except Exception as e:
        send_error(client_socket, f"Connection to the Server has failed! Error: {str(e)}")


def register_client(client_socket, client_handle, new_handle):
    global is_registered

    if new_handle not in registered_clients:
        registered_clients[new_handle] = {"socket": client_socket}
        send_message(client_socket, f"Welcome {new_handle}!")
        is_registered[client_handle] = True   
    else:
        send_error(client_socket, "Registration failed. Handle or alias already exists.")
    print("After registration, is_registered:", new_handle in registered_clients)

def store_file(client_socket, client_handle, filename):
    
    if client_handle not in registered_clients:
        send_error(client_socket, "Error: You must join the server first.")
        return

    
    full_path = os.path.join(os.getcwd(), filename)

    
    if not os.path.isfile(full_path):
        send_error(client_socket, "Error: File not found on the client side.")
        return

   
    if client_handle in registered_clients:
        with open(full_path, "rb") as file:
            file_data = file.read()
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            send_message(client_socket, f"{client_handle}<{timestamp}>: Uploaded {filename}")
            registered_clients[client_handle]["socket"].send(f"/store {filename} {file_data}".encode("utf-8"))
    else:
        send_error(client_socket, "Error: You must register first.")

def send_directory_list(client_socket, client_handle):
    
    print("Inside send_directory_list, is_registered:", client_handle in registered_clients)
    if client_handle in is_registered and is_registered[client_handle]:
        try:

            files = os.listdir()
            files_str = "\n".join(files)
            message = f"Server Directory\n{files_str}"
            send_message(client_socket, message)
        except Exception as e:
            send_error(client_socket, f"Error sending directory list: {str(e)}")
    else:
        send_error(client_socket, "Error: You must register first.")

def send_file(client_socket, client_handle, filename):
    
    if client_handle not in registered_clients:
        send_error(client_socket, "Error: You must join the server first.")
        return

    file_path = os.path.join(os.getcwd(), filename)  

    if os.path.isfile(file_path):
        with open(file_path, "rb") as file:
            file_data = file.read()
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            registered_clients[client_handle]["socket"].send(f"/get {filename} {len(file_data)} {timestamp}".encode("utf-8"))
            print(f"Server sending file: {filename}")
            registered_clients[client_handle]["socket"].sendall(file_data)
    else:
       
        send_error(client_socket, f"Error: File not found in the server - {filename}")
        print(f"File not found: {filename}")


def send_help(client_socket):
    help_message = """
    List of Commands:
    ----------------------------
    /join <server_ip_add> <port>
    /leave
    /register <handle>
    /store <filename>
    /dir
    /get <filename>
    /?
    ----------------------------
    """
    send_message(client_socket, help_message)

def send_message(client_socket, message):
    client_socket.send(message.encode("utf-8"))

def send_error(client_socket, error_message):
    send_message(client_socket, f"{error_message}")

def remove_client(client_handle):
    if client_handle in registered_clients:
        registered_clients[client_handle]["socket"].close()
        del registered_clients[client_handle]

def main():

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((SERVER_IP, SERVER_PORT))
    server.listen(5)

    print(f"Server listening on {SERVER_IP}:{SERVER_PORT}")

    while True:
        client_socket, client_address = server.accept()
        threading.Thread(target=handle_client, args=(client_socket, client_address, f"Client-{client_address}")).start()

if __name__ == "__main__":
    is_registered = {}
    main()
