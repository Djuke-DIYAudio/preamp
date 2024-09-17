#!/usr/bin/env python

import logging
import threading
import serial
import time

# Serial read thread
def read_serial_data(preamp):
    serial_log=open(preamp._logname, 'ab')
    serial_log.write("\nDjuke Preamp log file\n".encode())

    preamp._ser.flushInput()

    while True:
        reading = preamp._ser.read()
        serial_log.write(reading)
        serial_log.flush()
	
        if preamp._stop_thread:
#            serial_log.write("Thread stopped\n")
            serial_log.flush()
            serial_log.close()
            break

# DjukePreamp serial connection
class PreampClient(object):
    def __init__(self, port="/dev/ttyAMA0", baudrate="115200", timeout=1, logname=""):
        self._ser = serial.Serial()
        self._ser.port = port
        self._ser.baudrate = baudrate
        self._ser.timeout = timeout
        self._stop_thread = False
        self._logname = logname

    def open(self):
        self._ser.open()

        if (self._ser.isOpen()):
            print("Serial port: " + self._ser.port + " opened")
            self._thread = threading.Thread(target=read_serial_data, args=(self, ))
            self._thread.start()
            self.write("")
            self.version()

    def close(self):
        self._stop_thread = True
        time.sleep(1)
        self._ser.close()
        
    def write(self, command):
        if (self._ser.isOpen()):
            self._ser.flushOutput()
            time.sleep(0.05)
            self._ser.write((command + "\n").encode())
            time.sleep(0.05)

    # Request preamp version
    def version(self):
        self.write("ver")

if __name__ == "__main__":
    import sys

    try:
        preamp = PreampClient(logname="DjukePreamp-serial.log")
        preamp.open()
        preamp.close()

    except (KeyboardInterrupt, SystemExit):
        print("Keyboard or system exit interrupt");
        preamp.close();
        sys.exit();

    # Catch all other non-exit errors
    except Exception as e:
        print(e)
        preamp.close()
        sys.exit(1)

    except:
        preamp.close()
        sys.exit(0)
