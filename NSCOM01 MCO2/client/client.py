import socket
import os

# Define global variables
BUFFER_SIZE = 8192

# Receive and return the server's response
def receive_response(sock):
    response = sock.recv(BUFFER_SIZE).decode()
    print(response.strip())
    print()
    return response

# Request passive mode from the server and establish a data connection
def handle_pasv(sock):
    sock.sendall("PASV\r\n".encode())
    response = receive_response(sock)
    parts = response.split('(')[1].split(')')[0].split(',')
    data_ip = '.'.join(parts[:4])
    data_port = (int(parts[4]) << 8) + int(parts[5])
    data_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    data_sock.connect((data_ip, data_port))
    return data_sock

# Send LIST command and print the directory listing from the data connection
def list_directory(pasv_sock, control_sock):
    control_sock.sendall("LIST\r\n".encode())
    
    # Immediate attempt to read the directory listing from the data connection.
    directory_listing = ''
    while True:
        data = pasv_sock.recv(BUFFER_SIZE).decode()
        if not data:
            break  # No more data; the listing is complete.
        directory_listing += data
    pasv_sock.close()  # Close the data connection

    if directory_listing:
        print(directory_listing.strip())  # Print the accumulated directory listing.
    else:
        print("No directory listing received.")

    # Expecting a final response from the control connection, indicating the operation is complete.
    final_response = receive_response(control_sock)
    if "226" in final_response:
        print("Directory listing completed successfully.")
    else:
        print("Unexpected server response after LIST operation.")

# Download a file from the server
def download_file(pasv_sock, control_sock, filename):
    control_sock.sendall(f"RETR {filename}\r\n".encode())
    response = receive_response(control_sock)  # Server response before starting file transfer
    
    with open(filename, 'wb') as file:
        total_bytes = 0
        while True:
            data = pasv_sock.recv(BUFFER_SIZE)
            if not data:
                break
            file.write(data)
            total_bytes += len(data)
        print(f"Total bytes downloaded: {total_bytes}")

    pasv_sock.close()  # Ensure the data connection is closed after the operation
    receive_response(control_sock)  # Final server response after file transfer


# Upload a file to the server
def upload_file(pasv_sock, control_sock, filename):
    control_sock.sendall(f"STOR {os.path.basename(filename)}\r\n".encode())
    receive_response(control_sock)  # Server response before starting file transfer
    with open(filename, 'rb') as file:
        total_bytes = 0
        while True:
            data = file.read(BUFFER_SIZE)
            if not data:
                break
            pasv_sock.sendall(data)
            total_bytes += len(data)
        print(f"Total bytes downloaded: {total_bytes}")
    pasv_sock.close()  # Ensure the data connection is closed after the operation
    receive_response(control_sock)  # Final server response after file transfer

# Send a command to the server and print the response
def send_command(sock, command):
    sock.sendall(f"{command}\r\n".encode())
    return receive_response(sock)

# Main function to start the client
def main():
    server_host = input("Enter FTP Server Hostname or IP: ")
    server_port = int(input("Enter FTP Server Port: "))

    control_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    control_sock.connect((server_host, server_port))
    receive_response(control_sock)  # Display welcome message

    print("Type 'HELP' to see the list of available commands.")

    logged_in = False

    while True:
        print()
        command = input("Enter FTP Command: ").strip()
        cmd_parts = command.split()
        cmd_upper = cmd_parts[0].upper()

        if cmd_upper == "HELP":
            send_command(control_sock, command)
            continue

        if cmd_upper == "QUIT":
            send_command(control_sock, "QUIT")
            break

        if not logged_in and cmd_upper in ["USER", "PASS"]:
            response = send_command(control_sock, command)
            if "230" in response:  # Logged in successfully
                logged_in = True
                # Set FTP transfer settings in the background after successful login
                print("Configuring transfer settings...\n")
                send_command(control_sock, "TYPE I")
                send_command(control_sock, "MODE S")
                send_command(control_sock, "STRU F")
            continue

        if not logged_in:
            print("332 Please log in first.")
            continue

        if cmd_upper == "PASV":
            pasv_sock = handle_pasv(control_sock)
            continue

        if cmd_upper == "LIST":
            if 'pasv_sock' not in locals():
                print("425 Enter PASV command first.")
            else:
                list_directory(pasv_sock, control_sock)
                del pasv_sock  # Ensure the PASV connection is closed
            continue

        if cmd_upper == "RETR" and len(cmd_parts) > 1:
            if 'pasv_sock' not in locals():
                print("425 Enter PASV command first.")
            else:
                download_file(pasv_sock, control_sock, cmd_parts[1])
                del pasv_sock
            continue

        if cmd_upper == "STOR" and len(cmd_parts) > 1:
            if 'pasv_sock' not in locals():
                print("425 Enter PASV command first.")
            else:
                upload_file(pasv_sock, control_sock, cmd_parts[1])
                del pasv_sock
            continue

        send_command(control_sock, command)

    control_sock.close()

if __name__ == "__main__":
    main()
