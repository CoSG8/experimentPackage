# LC_GoPro_Arduino.py
# ver. 1.1  2020.04.15

import pyfirmata
import sys
from urllib.request import urlopen
from time import sleep
import datetime
import signal
import http

GOPRO_IP = "10.5.5.9"
UDP_PORT = 8554
KEEP_ALIVE = 2.5
MESSAGE = "_GPHD_:0:0:2:0.000000"

Arduino_port = "COM8"
Ard_pin_out = 4
Ard_pin_in = 5

URL_PREFIX       = f"http://{GOPRO_IP}/gp/gpControl"
URL_STREAM       = f"{URL_PREFIX}/execute?p1=gpStream&a1=proto_v2&c1=restart"
URL_RECORD_START = f"{URL_PREFIX}/command/shutter?p=1"
URL_RECORD_STOP  = f"{URL_PREFIX}/command/shutter?p=0"

ard = pyfirmata.Arduino(Arduino_port)
it = pyfirmata.util.Iterator(ard)
it.start()

ard.digital[Ard_pin_in].mode = pyfirmata.INPUT

def live():
    
    print("Press Ctl+C to quit.")
    print("Press push the button to start/stop recording.")

    on = False
    while True:
        if ard.digital[Ard_pin_in].read() == True:
            if on == False:
                on = not on
                record(on)
                led(on)
            else:
                on = not on
                record(on)
                led(on)
                sleep(1)


def record(on):

    urlopen(URL_RECORD_START if on else URL_RECORD_STOP).read()
    print(f"Record {str(on)} {datetime.datetime.now()}")

def led(on):
    if on:
        sleep(5)
        ard.digital[Ard_pin_out].write(1)
        sleep(3)
        ard.digital[Ard_pin_out].write(0)

def quit_gopro(signal, frame):

    sys.exit(0)

if __name__ == '__main__':

    signal.signal(signal.SIGINT, quit_gopro)
    live()