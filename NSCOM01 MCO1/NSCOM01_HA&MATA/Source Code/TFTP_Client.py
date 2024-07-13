from datetime import datetime
import os
import time
import traceback
import sys
from socket import socket, setdefaulttimeout, AF_INET, SOCK_DGRAM, gethostname, gethostbyname

DEV = True
LOG = True
LOG_FILE = 'tftp_log.txt'

TFTP_MODES = {
    'unknown': 0,
    'netascii': 1,
    'octet': 2,
    'mail': 3
}

TFTP_OPCODES = {
    'unknown': 0,
    'read': 1,  # RRQ
    'write': 2,  # WRQ
    'data': 3,  # DATA
    'ack': 4,  # ACKNOWLEDGMENT
    'error': 5
}  # ERROR


class TFTPLog:

    @staticmethod
    def log(action, msg=None, params=None):
        if LOG:
            try:
                ft_message = "Logged action: {0}\nDate: {1}\nParams: {2}\nMessage: {3}\n\n".format(
                    action, datetime.today(), params, msg)
                if DEV:
                    print(ft_message)

                with open(LOG_FILE, 'a+') as logfile:
                    logfile.write(ft_message)

            except Exception as logex:
                print("Logex: %s" % (logex))


class TFTPPackets:

    def __init__(self):
        #self.log = TFTPLog
        self.opcodes = TFTP_OPCODES
        self.modes = TFTP_MODES
        self.to_int = lambda args: [ord(a) for a in args]
        self.to_bytes = bytearray

    def join(self, *arrs):
        res = []

        try:
            for arr in arrs:
                if not isinstance(arr, list):
                    arr = [arr]
                res += arr
        except Exception as err:
            print("join", err)
        return res

    def request_packet(self, filename, mode, opcode):
        try:
            return self.to_bytes(self.join(0, opcode, self.to_int(filename), 0, self.to_int(mode), 0))
        except Exception as err:
            print("request_packet", err)
            TFTPLog.log(action="request_packet", params=(filename, mode, opcode), msg="Err: %s" % err)

    def ack_packet(self, packetnr):
        try:
            return self.to_bytes(self.join(0, self.opcodes['ack'], ((packetnr >> 8) & 0xff), (packetnr & 0xff)))
        except Exception as err:
            print("ack_packet", err)
            TFTPLog.log(action="ack_packet", params=(packetnr), msg="Creating ack packet: {0}\nErr: {1}".format(packetnr, err))

    def data_packet(self, packetnr, buffer):
        try:
            encoding = 'latin1'
            return self.to_bytes(self.join(0, self.opcodes['data'], (packetnr >> 8) & 0xff, packetnr & 0xff,
                                           self.to_int(buffer.decode(encoding))))
        except Exception as err:
            print("data_packet", err)
            TFTPLog.log(action="data_packet",
                     params=(packetnr, buffer),
                     msg="Calling data packet, data: {0}\nErr: {1}, traceback: {2}".format(buffer, err,
                                                                                                traceback.format_exc()))


class TFTPException(Exception):

    def __init__(self, message):
        self.message = message

    def __str__(self):
        return str(self.message)


class TFTPClient:

    NOT_FOUND = 'Could not find file'
    TIME_OUT = 'timed out'
    MAX_RETRY_COUNT = 0xff  # 255 max retry count during transmission
    MAX_RUNS = 0xffffffff  # 4294967295 total write() runs before shutting down prog
    MAX_TRANSFER = 2 << 14  # 32768 bytes is max transfer
    HEADER_SIZE = 2 << 1  # 4 bytes for header
    DATA_SIZE = 2 << 8  # 512 bytes for data
    BLK_SIZE = DATA_SIZE + HEADER_SIZE  # 516 bytes

    def __init__(self, host, port=None):
        self.modes, self.opcodes = (TFTP_MODES, TFTP_OPCODES)
        setdefaulttimeout(5)
        self.addr = (host, int(port) if port else 69)
        self.socket = socket(AF_INET, SOCK_DGRAM)
        #self.log = TFTPLog

        tftp_packets = TFTPPackets()
        self.ack_packet = tftp_packets.ack_packet
        self.request_packet = tftp_packets.request_packet
        self.data_packet = tftp_packets.data_packet

    def __str__(self):
        return "%s:%s" % (self.addr)

    def __del__(self):
        #if hasattr(self, 'log'):
        TFTPLog.log(action="__del__", params=str(self), msg="Calling tftpclient.destructor")

        if hasattr(self, 'socket'):
            self.socket.close()

    def read(self, remote, local=None, mode='octet'):
        success = False
        try:
            if not self.socket:
                self.socket = socket(AF_INET, SOCK_DGRAM)

            if not remote:
                raise TFTPException("Remote file name cannot be empty.")

            if not local:
                local = remote

            TFTPLog.log(action="read", params=(remote, local, mode),
                        msg=f"Initiating RRQ request to: {self.addr}")

            opcode, packetnr = self.opcodes['read'], 1
            snd_buffer = self.request_packet(remote, mode, opcode)
            self.socket.sendto(snd_buffer, self.addr)

            rcv_total, retry_count = 0, 0
            with open(local, 'wb+') as f:
                while True:
                    rcv_buffer, (host, port) = self.socket.recvfrom(self.BLK_SIZE)
                    rcv_total += len(rcv_buffer)

                    if rcv_buffer[1] == self.opcodes['error']:
                        raise TFTPException(f"Error received: {rcv_buffer[4:].decode()}")

                    packet_block_number = ((rcv_buffer[2] << 8) & 0xff00) + rcv_buffer[3]
                    if packet_block_number == packetnr:
                        f.write(rcv_buffer[4:])
                        packetnr += 1

                    snd_buffer = self.ack_packet(packet_block_number)
                    self.socket.sendto(snd_buffer, (host, port))
                    TFTPLog.log(action="ACK sent", params=(packet_block_number), 
                                msg=f"ACK packet sent for block number {packet_block_number}.")

                    if len(rcv_buffer) < 512:
                        TFTPLog.log(action="read finished", 
                                    msg=f"Last packet received. Total bytes received: {rcv_total}")
                        success = True
                        break
        
        except ConnectionResetError as cre:
            print("Failure to contact server. The server may be down or not reachable.")
            TFTPLog.log(action="Failure to Contact Server", msg=f"ConnectionResetError: {cre}. The server may be down or not reachable.")

        except TFTPException as terr:
            TFTPLog.log(action="read: tftpexception", params=(remote, local, mode),
                        msg=f"Error: {terr}")

        except Exception as err:
            TFTPLog.log(action="read: exception", params=(remote, local, mode),
                        msg=f"Exception occurred: {str(err)}\nTraceback: {traceback.format_exc()}")

        return success



    def write(self, local, remote=None, mode='octet'):
        try:
            if not self.socket:
                self.socket = socket(AF_INET, SOCK_DGRAM)

            success = False

            if not local:
                raise TFTPException("Local file cannot be empty.")

            if not remote:
                remote = local

            file = open(local, 'rb+')
            file_buffer = file.read()

            TFTPLog.log(action='write', params=(remote, local, mode),
                    msg="Initiating WRQ request {0}/{1} of size {2} KB.".format(self.addr, remote,
                                                                                round(len(file_buffer) / 1024)))

            opcode, packetnr = self.opcodes['write'], 0
            snd_buffer = self.request_packet(remote, mode, opcode)

            self.socket.sendto(snd_buffer, self.addr)
            (rcv_buffer, (host, port)) = self.socket.recvfrom(self.BLK_SIZE)

            start, retry_count, total_runs, start_time, timeout = 0, 0, 0, time.time(), False
            while True:
                try:
                    if total_runs == self.MAX_RUNS:
                        print("Max run count reached!")
                        break

                    if packetnr % 5000 == 0:
                        print("Total {0} sent: {1}, execution time: {2} sec".format('KB', self.DATA_SIZE * total_runs,
                                                                                    time.time() - start_time))

                    if not host and port:
                        raise TFTPException("Host and port are invalid: %s:%s" % (host, port))

                    if rcv_buffer[1] == self.opcodes['error']:
                        raise TFTPException(rcv_buffer[4:].decode())

                    if rcv_buffer[1] == self.opcodes['ack'] and (((rcv_buffer[2] << 8) & 0xff00) + rcv_buffer[3]) == packetnr & 0xffff:
                        TFTPLog.log(action="ACK received", params=(packetnr), 
                                    msg="ACK packet received for block number {0}.".format(packetnr))
                        if not timeout:
                            buffer = file_buffer[start: (self.DATA_SIZE + start)]
                            packetnr += 1

                        snd_buffer = self.data_packet(packetnr, buffer)
                        self.socket.sendto(snd_buffer, (host, port))

                        (rcv_buffer, (host, port)) = self.socket.recvfrom(self.BLK_SIZE)
                        timeout = False
                        start += self.DATA_SIZE

                    if len(snd_buffer) < self.BLK_SIZE:
                        TFTPLog.log(action="write end", msg="Ending write()")
                        break

                    total_runs += 1

                except Exception as err:
                    message = "Packetnr: {0}, retry count: {1}, header: {2}, error: {3}\ntraceback: {4}"
                    TFTPLog.log(action="write exception", params=(remote, local, mode),
                            msg=message.format(packetnr, retry_count, rcv_buffer[:4], err,
                                                traceback.format_exc()))

                    if self.TIME_OUT in err.args:
                        timeout = True
                        retry_count += 1

                        if retry_count >= self.MAX_RETRY_COUNT:
                            print("Max retried sends... leaving")
                            break
                        else:
                            TFTPLog.log(action="writetimeout exception", params=(remote, local, mode),
                                    msg=message.format(packetnr, retry_count, rcv_buffer[:4], err,
                                                        traceback.format_exc()))

            success = True
            TFTPLog.log(action="write success", params=(remote, local, mode),
                    msg="Success in writing file {0} to host {1}, total bytes sent: {2}, total retry counts: {3}, execution time: {4} seconds".format(
                        remote, self.addr, len(file_buffer), retry_count, time.time() - start_time))

        except ConnectionResetError as cre:
            print("Failure to contact server. The server may be down or not reachable.")
            TFTPLog.log(action="Failure to Contact Server", msg=f"ConnectionResetError: {cre}. The server may be down or not reachable.")
        
        except TFTPException as terr:
            TFTPLog.log(action="write: tftpexception", params=(remote, local, mode),
                    msg="Error: {0}, traceback: {1}".format(err, traceback.format_exc()))

        except Exception as err:
            TFTPLog.log(action="write: outerexception", params=(remote, local, mode),
                    msg="Error: {0}, traceback: {1}".format(err, traceback.format_exc()))

        finally:
            if file:
                file.close()

        return success


def main():
    try:
        flag = True
        buffer = []

        host_name = gethostname()
        addr = (gethostbyname(host_name), host_name)

        (host, port) = input('Enter host:port --> ').split(':')

        c = TFTPClient(host, port)

        if c:
            buffer.append(
                'Connecting from host: {0} to host: {1}:{2}, mode: {3}'.format(addr, host, port, 'octet'))

            for p in range(len(buffer)):
                print(buffer.pop())
        else:
            flag = False
            print("Could not create TFTPClient() with: {0}:{1}".format(host, port))

        while flag:

            method = input('\nEnter read/r, write/w, quit/q --> ').lower()

            if method in ['quit', 'q']:
                buffer.append("Flag off")
                flag = False

            else:
                remote_file, local_file = input('Enter remote filename --> '), input('Enter local filename --> ')

                if method in ['read', 'r']:
                    if not remote_file:
                        raise Exception("Remote file is invalid!");
                    else:
                        if not local_file:
                            local_file = remote_file
                        if c.read(remote_file, local_file):
                            if '.' in os.path.splitext(local_file)[-1]:
                                if os.path.isfile(local_file):
                                    buffer.append(
                                        "Successfully read file {0} from server {1}:{2} to path {3}/{4}".format(
                                            remote_file, host, port, os.getcwd(), local_file))

                elif method in ['write', 'w']:

                    if not os.path.isfile(local_file):
                        raise Exception("File %s not found!" % local_file)
                    else:
                        if not remote_file:
                            remote_file = local_file
                        if c.write(local_file, remote_file):
                            buffer.append(
                                "Successfully wrote file {0} from path {1} to server {2}:{3}/{4}".format(
                                    local_file, os.getcwd(), host, port, remote_file))

            for i in range(len(buffer)):
                print(buffer.pop())

    except Exception as err:
        print("main.err: %s" % err)

    finally:
        print("Leaving. . .")


if __name__ == '__main__':
    main()
