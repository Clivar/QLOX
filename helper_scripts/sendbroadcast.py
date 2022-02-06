import socket
import time
import sys

DISCOVERY_PORT = 49999


def find_devices(bind_ip='', timeout=1) -> list:
    """
    Scans the local subnet for Magic Home devices by using a broadcast

    bind_ip -- in case the host is using multiple network interfaces,
    specify the ip of that local interface
    timeout -- specify how long it should wait after the broadcast
    for devices to respond
    """
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((bind_ip, DISCOVERY_PORT))
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    msg = "HELLOQLOX".encode('ascii')

    # set the time at which we will quit the search
    quit_time = time.time() + timeout

    # outer loop for query send
    while True:
        if time.time() > quit_time:
            break
        # send out a broadcast query
        sock.sendto(msg, ('<broadcast>', DISCOVERY_PORT))
        while True:
            sock.settimeout(3)
            try:
                data, addr = sock.recvfrom(64)
            except socket.timeout:
                data = None
                if time.time() > quit_time:
                    break

            if data is None:
                continue
            if data == msg:
                continue
            print("got a message!")
            data = data.decode('ascii')
            print(data)
            sock.close()
            return

def main():
    devices = find_devices(bind_ip='10.10.20.151')


if __name__ == '__main__':
    main()