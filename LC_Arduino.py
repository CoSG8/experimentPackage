# LC_Arduino.py
# ver. 1.0  2020.07.13

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
Ard_pin_in_1 = 7
Ard_pin_in_2 = 5

URL_PREFIX       = f"http://{GOPRO_IP}/gp/gpControl"
URL_STREAM       = f"{URL_PREFIX}/execute?p1=gpStream&a1=proto_v2&c1=restart"
URL_RECORD_START = f"{URL_PREFIX}/command/shutter?p=1"
URL_RECORD_STOP  = f"{URL_PREFIX}/command/shutter?p=0"

ard = pyfirmata.Arduino(Arduino_port)


def live():
    
    print("Press Ctl+C to quit.")
    print("Press push the button to start/stop recording.")

    on_1 = False
    on_2 = False
    while True:
        if ard.digital[Ard_pin_in_1].read() == True:
            if on_1 == False:
                on_1 = not on_1
                record(on_1)
                sleep(5)
                led(on_1)
        else:
            if on_1:
                on_1 = not on_1
                record(on_1)
                led(on_1)
                sleep(1)

        if ard.digital[Ard_pin_in_2].read() == True:
            if on_2 == False:
                on_2 = not on_2
                led(on_2)
                on_2 = not on_2

def record(on):
    # urlopen(URL_RECORD_START if on else URL_RECORD_STOP).read()
    print(f"Record {str(on)} {datetime.datetime.now()}")

def led(on):
    if on:
        ard.digital[Ard_pin_out].write(1)
        sleep(3)
        ard.digital[Ard_pin_out].write(0)

def quit_gopro(signal, frame):

    sys.exit(0)

if __name__ == '__main__':
    it = pyfirmata.util.Iterator(ard)
    it.start()
    ard.digital[Ard_pin_in_1].mode = pyfirmata.INPUT
    ard.digital[Ard_pin_in_2].mode = pyfirmata.INPUT
    
    signal.signal(signal.SIGINT, quit_gopro)
    live()