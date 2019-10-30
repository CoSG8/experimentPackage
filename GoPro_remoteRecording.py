#!/usr/bin/env python3

import sys
import socket
from urllib.request import urlopen
import subprocess
from time import sleep
import datetime
import signal
import http
import multiprocessing

# GPH 6/0.160
GOPRO_IP = "10.5.5.9"
UDP_PORT = 8554
KEEP_ALIVE = 2.5
MESSAGE = "_GPHD_:0:0:2:0.000000"

URL_PREFIX       = f"http://{GOPRO_IP}/gp/gpControl"
URL_STREAM       = f"{URL_PREFIX}/execute?p1=gpStream&a1=proto_v2&c1=restart"
URL_RECORD_START = f"{URL_PREFIX}/command/shutter?p=1"
URL_RECORD_STOP  = f"{URL_PREFIX}/command/shutter?p=0"

CMD_VIEW = f"ffplay -loglevel panic -fflags nobuffer -f:v mpegts -probesize 8192 udp://:{UDP_PORT}"

def live():

    urlopen(URL_STREAM).read()
    subprocess.Popen(CMD_VIEW, shell=True)
    multiprocessing.Process(target=keep_alive).start()

    print("Press Ctl+C to quit.")
    print("Press ENTER to start/stop recording.")

    on = False
    while True:
        input()
        on = not on
        record(on)

def keep_alive():

    while True:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(bytes(MESSAGE, "utf-8"), (GOPRO_IP, UDP_PORT))
        sleep(KEEP_ALIVE)

def record(on):

    urlopen(URL_RECORD_START if on else URL_RECORD_STOP).read()
    print(f"Record {str(on)} {datetime.datetime.today()}")


def quit_gopro(signal, frame):

    sys.exit(0)

if __name__ == '__main__':

    signal.signal(signal.SIGINT, quit_gopro)
    live()