import socket
import threading
import os
import shutil

# Define global variables
HOST = '127.0.0.1'
PORT = 21
BUFFER_SIZE = 8192
SERVER_ROOT = os.getcwd()  # Directory where server files are stored

# Define username and password
USER_INFO = {'john': '1234', 
             'jane': '5678', 
             'joe': 'qwerty'}

# Define global variables to track transfer settings
TRANSFER_MODE = "STREAM"  # Default transfer mode
TRANSFER_TYPE = "BINARY"  # Default transfer type
FILE_STRUCTURE = "FILE"   # Default file structure

# Function to send PASV response with passive port information
def handle_pasv(conn):
    pasv_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    pasv_socket.bind((HOST, 0))  # Automatically choose a free port.
    pasv_socket.listen(1)
    pasv_port = pasv_socket.getsockname()[1]  # Get the port chosen for passive mode
    print(f"Passive mode port: {pasv_port}")  # Print the passive mode port
    conn.send(f"227 Entering Passive Mode (127,0,0,1,{pasv_port >> 8},{pasv_port & 0xFF}).\r\n".encode())
    return pasv_socket

# Function to list directory contents
def list_directory(data_conn, current_directory):
    try:
        files = os.listdir(current_directory)
        formatted_list = '\r\n'.join(files) + '\r\n'
        data_conn.sendall(formatted_list.encode())
    except Exception as e:
        print(f"Error listing directory: {e}")

# Function to retrieve_file from the server
def retrieve_file(data_conn, file_path):
    try:
        with open(file_path, 'rb') as file:
            if TRANSFER_MODE == "STREAM":
                if TRANSFER_TYPE == "BINARY":
                    data_conn.sendfile(file)
                else:  # ASCII mode
                    data = file.read(BUFFER_SIZE)
                    while data:
                        data_conn.send(data)
                        data = file.read(BUFFER_SIZE)
            else:  # Block mode
                chunk_size = BUFFER_SIZE
                while True:
                    chunk = file.read(chunk_size)
                    if not chunk:
                        break
                    data_conn.sendall(chunk)
    except Exception as e:
        print(f"Error sending file: {e}")

# Function to store a file on the server
def store_file(data_conn, file_path):
    try:
        with open(file_path, 'wb') as file:
            if TRANSFER_MODE == "STREAM":
                if TRANSFER_TYPE == "BINARY":
                    while True:
                        data = data_conn.recv(BUFFER_SIZE)
                        if not data:
                            break
                        file.write(data)
                else:  # ASCII mode
                    while True:
                        data = data_conn.recv(BUFFER_SIZE).decode()
                        if not data:
                            break
                        file.write(data.encode())
            else:  # Block mode
                while True:
                    chunk = data_conn.recv(BUFFER_SIZE)
                    if not chunk:
                        break
                    file.write(chunk)
    except Exception as e:
        print(f"Error receiving file: {e}")

# Function to handle client connections
def handle_client(conn, addr):
    print(f"Connection established with {addr}")
    conn.send(b"220 Welcome to NSCOM01 FTP Server!\r\n")
    authenticated = False
    current_directory = SERVER_ROOT
    username = ""
    pasv_socket = None 

    while True:
        cmd_full = conn.recv(BUFFER_SIZE).decode().strip()
        if not cmd_full:
            break
        command, *args = cmd_full.split()
        command = command.upper()

        if command == "HELP":
            # Send a help message listing available commands
            help_message = (
                "--------------------------------------------------\r\n"
                "214 - The following commands are available:\r\n\r\n"
                "USER {username} - Log in with username\r\n"
                "PASS {password} - Log in with password\r\n"
                "PWD - Print working directory\r\n"
                "CWD {directory} - Change working directory\r\n"
                "CDUP - Move to parent directory\r\n"
                "MKD {directory} - Make a new directory\r\n"
                "RMD {directory} - Remove a directory\r\n"
                "PASV - Enter passive mode\r\n"
                "LIST - List files and directories\r\n"
                "RETR {filename} - Retrieve a file from the server\r\n"
                "STOR {filename} - Store a file on the server\r\n"
                "DELE {filename} - Delete a file\r\n"
                "QUIT - Disconnect\r\n"
                "HELP - Returns available commands for the client\r\n\r\n"
                "214 Help OK.\r\n"
                "--------------------------------------------------\r\n"
            )
            conn.send(help_message.encode())
            continue

        # Handle USER authentication
        if command == "USER":
            username = args[0] if args else ""
            if username in USER_INFO:
                conn.send(b"331 User name okay, need password.\r\n")
            else:
                conn.send(b"430 Invalid username or password.\r\n")

        # Handle PASSWORD authentication
        elif command == "PASS" and not authenticated:
            password = args[0] if args else ""
            if username in USER_INFO and USER_INFO[username] == password:
                authenticated = True
                conn.send(b"230 User logged in, proceed.\r\n")
            else:
                conn.send(b"430 Invalid username or password.\r\n")

        # Handle QUIT command
        elif command == "QUIT":
            conn.send(b"221 Closing socket...\r\n")
            break
        
        # Handle authenticated commands
        elif authenticated:
            
            # PWD - Print Working Directory
            if command == "PWD":
                conn.send(f"257 \"{current_directory}\" is current directory.\r\n".encode())

            # CWD - Change Working Directory
            elif command == "CWD":
                target_dir = os.path.join(current_directory, *args) if args else SERVER_ROOT
                if os.path.exists(target_dir):
                    current_directory = target_dir
                    conn.send(b"250 Directory changed.\r\n")
                else:
                    conn.send(b"550 Directory not found.\r\n")

            # CDUP - Change to Parent Directory
            elif command == "CDUP":
                current_directory = os.path.dirname(current_directory)
                conn.send(b"200 Command okay.\r\n")

            # MKD - Make Directory
            elif command == "MKD":
                dir_path = os.path.join(current_directory, *args)
                os.makedirs(dir_path, exist_ok=True)
                conn.send(b"257 Directory created.\r\n")

            # RMD - Remove Directory
            elif command == "RMD":
                dir_path = os.path.join(current_directory, *args)
                shutil.rmtree(dir_path, ignore_errors=True)
                conn.send(b"250 Directory deleted.\r\n")

            # DELE - Delete File
            elif command == "DELE" and args:
                file_path = os.path.join(current_directory, args[0])
                try:
                    os.remove(file_path)
                    conn.send(b"250 File deleted.\r\n")
                except FileNotFoundError:
                    conn.send(b"550 File not found.\r\n")
            
            # PASV - Enter Passive Mode
            elif command == "PASV":
                if pasv_socket:  # Close previous pasv_socket if exists
                    pasv_socket.close()
                pasv_socket = handle_pasv(conn)
            
            # LIST - Print Current List of Files
            elif command == "LIST":
                if pasv_socket:  # Ensure pasv_socket is defined
                    data_conn, _ = pasv_socket.accept()
                    list_directory(data_conn, current_directory)
                    data_conn.close()  # Make sure to close the data connection here
                    pasv_socket.close()  # Close the listening socket as well
                    pasv_socket = None  # Reset pasv_socket to None after use
                    conn.send(b"226 Closing data connection. Directory send OK.\r\n")
                else:
                    conn.send(b"425 Use PASV command first.\r\n")

            # RETR - Retrieve/Get File(s)
            elif command == "RETR" and args and pasv_socket:
                file_path = os.path.join(current_directory, args[0])
                if os.path.isfile(file_path):
                    data_conn, _ = pasv_socket.accept()
                    conn.send(b"150 Opening binary mode data connection.\r\n")
                    retrieve_file(data_conn, file_path)
                    data_conn.close()
                    conn.send(b"226 Closing data connection. Retrieved file successfully.\r\n")
            
            # STOR - Upload Data at the Server Site
            elif command == "STOR" and args and pasv_socket:
                file_path = os.path.join(current_directory, args[0])
                data_conn, _ = pasv_socket.accept()
                conn.send(b"150 Ok to send data.\r\n")
                store_file(data_conn, file_path)
                data_conn.close()
                conn.send(b"226 Closing data connection. Stored file successfully.\r\n")

            # TYPE - Transfer Mode (Binary)
            elif command == "TYPE":
                type_mode = args[0].upper() if args else ""
                if type_mode == "I":
                    TRANSFER_TYPE = "BINARY"
                    conn.send(b"200 Switching to Binary mode.\r\n")
                else:
                    # Respond with an error if any mode other than binary is requested
                    conn.send(b"504 Command parameter not supported.\r\n")

            # MODE - Transfer Mode (Stream)
            elif command == "MODE":
                mode = args[0].upper() if args else ""
                if mode == "S":  # Stream mode
                    TRANSFER_MODE = "STREAM"
                    conn.send(b"200 Mode set to Stream.\r\n")
                else:
                    conn.send(b"504 Command parameter not supported.\r\n")

            # STRU - File Transfer Stucture
            elif command == "STRU":
                structure = args[0].upper() if args else ""
                if structure == "F":  # File structure
                    FILE_STRUCTURE = "FILE"
                    conn.send(b"200 Structure set to File.\r\n")
                elif structure == "R": # Record structure
                    FILE_STRUCTURE = "RECORD"
                    conn.send(b"200 Structure set to Record.\r\n")
                else:
                    conn.send(b"504 Command parameter not supported.\r\n")

            else:
                conn.send(b"502 Command not implemented.\r\n")
        else:
            conn.send(b"530 Please login with USER and PASS.\r\n")

    conn.close()

# Main function to start the server
def main():
    if not os.path.exists(SERVER_ROOT):
        os.makedirs(SERVER_ROOT)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen(5)
        print(f"Server listening on {HOST}:{PORT}")

        while True:
            try:
                conn, addr = server_socket.accept()
                threading.Thread(target=handle_client, args=(conn, addr)).start()
            except Exception as e:
                print(f"Error accepting connection: {e}")

if __name__ == "__main__":
    main()
