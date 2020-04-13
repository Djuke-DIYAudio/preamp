#!/usr/bin/env python

# Example program to interface the Volumio media player running on a Raspberry Pi
# with the serial console on the Djuke Preamplifier, this gives endless possibilities, like:
# - switching on the amplifier when the media player starts playing
# - automatically choose the correct input when the media player starts playing
# - display the song title on the Preamplifier display
# - switch off the amplifier when the media player stops playing
# - etc.
#
# The Volumio media player can be accessed from Python using SocketIO
#
# To install it on the Volumio raspbian distribution, connect using ssh to it and run these commands:
#
# (sudo apt-get update)
# sudo apt-get install python-pip
# sudo pip install python-SocketIO-client
#
# The connection to the Djuke preamplifier uses python-serial, if it is not already installed, install it with
#
# sudo apt-get install python-serial
# 

import time
import sys
from VolumioClient import VolumioClient
from PreampClient import PreampClient
import unicodedata

import logging
import logging.handlers

# Logging parameter
#LOG_FILENAME = "/var/log/DjukePreamp.log"
LOG_FILENAME = "DjukePreamp.log"

# Configure logging to log to a file, making a new file at midnight and keeping the last 3 day's data
# Give the logger a unique name (good practice)
logger = logging.getLogger()
# Set the log level
logger.setLevel(logging.INFO)
# Make a handler that writes to a file, making a new file at midnight and keeping 3 backups
handler = logging.handlers.TimedRotatingFileHandler(LOG_FILENAME, when="midnight", backupCount=3)
# Format each log message like this
formatter = logging.Formatter('%(asctime)s %(levelname)s - %(message)s')
# Attach the formatter to the handler
handler.setFormatter(formatter)
# Attach the handler to the logger
logger.addHandler(handler)

# Add stdout to logger
#stdout_handler = logging.StreamHandler(sys.stdout)
#stdout_handler.setLevel(logging.INFO)
#logger.addHandler(stdout_handler)

# Add stderr to logger
#stderr_handler = logging.StreamHandler(sys.stderr)
#stderr_handler.setLevel(logging.ERROR)
#logger.addHandler(stderr_handler)

try:
    preamp = PreampClient(logname="DjukePreamp-serial.log")
    preamp.open()

    # Global variables with initial status and title
    status = 'unknown'
    title = 'unknown'

    # Define callback functions
    def status_changed(new_status):
        global status, title
        print('status_changed(): ' + status + ' -> ' + new_status)
        logger.info('status_changed(): ' + status + ' -> ' + new_status)
        if new_status == 'stop':
            logger.info("player stopped, remove preamp title")
            preamp.write("input set 9")
        if new_status == 'pause':
            logger.info("player paused, remove preamp title")
            preamp.write("input set 9")
        if new_status == 'play':
            # Optionally, switch on the preamp first, if playback just started, or set the input
            if status == 'stop':
#                logger.info("Player started, power on preamp")
#                preamp.write("power on")
                logger.info("Set preamp input to 9: Mediaplayer")
                preamp.write("input set 9")
        status = new_status

    def title_changed(new_title):
        global status, title
        if status == 'play':
            # Convert title to ascii, while trying to maintain readable titles
            title = unicodedata.normalize("NFD", new_title).encode('ascii', 'ignore')
            # Truncate to max 70 characters
            title = title[:70]
            print('title_changed(): ' + title)
            logger.info('title_changed(): ' + title)
            logger.info("Set preamp title to: " + title)
            preamp.write("title 9 " + title)

    # define volumio client with callback functions
    volumio = VolumioClient(host="localhost", port=3000)
    volumio.on_status_change(status_changed)
    volumio.on_title_change(title_changed)
    volumio.connect()

    # main loop
    while True:
    	volumio.wait()

except (KeyboardInterrupt, SystemExit):
    logger.info("Keyboard or system exit interrupt");
    preamp.close();
    sys.exit();

# Catch all other non-exit errors
except Exception as e:
    logger.error(e)
    sys.exit(1)

except:
    sys.exit(0)

