#!/bin/bash

# Script to install Djukepreamp.py as a background service on Volumio

# Python files will be installed here
mkdir -p /usr/local/Djuke

# Copy the files
cp DjukePreamp.py /usr/local/Djuke
cp VolumioClient.py /usr/local/Djuke
cp PreampClient.py /usr/local/Djuke
cp DjukePreamp.service /etc/systemd/system

# Install the service
systemctl enable DjukePreamp

# Following command can be used for testing
# service DjukePreamp start|stop|status
