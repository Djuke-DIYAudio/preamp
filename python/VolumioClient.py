#!/usr/bin/env python

# Class for connecting to Volumio and retrieving its playback status.
# You can define callback function that are called on a change of status, title, artist
#
# Uses python-socketio for connecting to Volumio and retrieving the status: https://pypi.org/project/python-socketio/
#

import socketio

class VolumioClient(object):
    def __init__(self, url="http://localhost:3000"):
        self._url = url
        self._status = ''
        self._title = ''
        self._artist = ''
        self._socketio = ''
        self._status_change_callback = ''
        self._title_change_callback = ''
        self._artist_change_callback = ''

    def on_status_change(self, callback):
        self._status_change_callback = callback

    def on_title_change(self, callback):
        self._title_change_callback = callback

    def on_artist_change(self, callback):
        self._artist_change_callback = callback

    # Callback function as response on 'getState', used to retrieve status, title, artist
    def on_push_state(self, *args):
#        print('getstate', args)
        if 'status' in args[0] and type(args[0]['status']) is str:
            new_status = args[0]['status']
            if new_status != self._status:
                if self._status_change_callback:
                    self._status_change_callback(new_status)
                self._status = new_status

                # Set empty title when playback is stopped to ensure also title callback is triggered when it starts again
                if new_status != 'play':
                    self._title = ''
        if 'title' in args[0] and type(args[0]['title']) is str:
            new_title = args[0]['title']
            if new_title != self._title:
                if self._title_change_callback:
                    self._title_change_callback(new_title)
                self._title = new_title

            # Set empty title when playback is stopped to ensure also title callback is triggered when it starts again
            if self._status != 'play':
                self._title = ''
        if 'artist' in args[0] and type(args[0]['artist']) is str:
            new_artist = args[0]['artist']
            if new_artist != self._artist:
                if self._artist_change_callback:
                    self._artist_change_callback(new_artist)
                self._artist = new_artist

    def on_connect(self):
        print("Connected to", self._url)

    def on_disconnect(self):
        print("Disconnected from", self._url)

    def connect(self):
        self._socketio = socketio.Client(logger=False)

        # Response handlers
        self._socketio.on('connect', self.on_connect)
        self._socketio.on('disconnect', self.on_disconnect)
        self._socketio.on('pushState', self.on_push_state)

        self._socketio.connect(self._url, transports=['websocket'])

    def wait(self):
        # getState command
        self._socketio.emit('getState')
        self._socketio.sleep(10)

if __name__ == "__main__":
    import sys
    import unicodedata

    status = 'unknown'

    # Define callback functions
    def status_changed(new_status):
        global status
        print('status_changed() status: ' +  status + '->' + new_status)
        status = new_status

    def title_changed(title):
        print('title_changed(): ' + title)

    def artist_changed(artist):
        print('artist_changed(): ' + artist)

    try:
        volumio = VolumioClient(url="http://localhost:3000")
        volumio.on_status_change(status_changed)
        volumio.on_title_change(title_changed)
        volumio.on_artist_change(artist_changed)
        volumio.connect()

        while True:
        	volumio.wait()

    except (KeyboardInterrupt, SystemExit):
        print("Keyboard or system exit interrupt");
        sys.exit();

    # Catch all other non-exit errors
    except Exception as e:
        print(e)
        sys.exit(1)

    except:
        sys.exit(0)


