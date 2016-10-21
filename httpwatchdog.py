#!/usr/bin/python3
#
# httpwatchdog.py
#
# Small HTTP server that works as a systemd notifier
#
# Listens on 127.0.0.1:8080, and restarts the systemd service watchdog
# timer on any HTTP request.
#
# Used to automatically restart a HTTP kiosk when the page dies
# (browser problem/network problem) instead of hanging.
#
# Depends on the sdnotify library available here:
#   https://github.com/bb4242/sdnotify
#   commit: cc7da0c83a27e42619f0d800b11ef7bf7f82e4bb
#
# Example systemd service:
# [Service]
# User=kiosk
# Group=kiosk
# ExecStart=/usr/local/bin/startkiosk
# Restart=always
# WatchdogSec=120
# NotifyAccess=all
#
# Make sure whatever is in ExecStart starts this and that there is a
# web-page loaded that perodically calls this service.
#
# Copyright (c) 2015 James Klassen
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies of this Software or works derived from this Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.


from http.server import HTTPServer, BaseHTTPRequestHandler
import sdnotify

notifier = sdnotify.SystemdNotifier()

class MyHTTPRequestHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		notifier.notify("WATCHDOG=1")
		self.send_response(200, 'Ok')
		self.send_header('Access-Control-Allow-Origin', '*')
		self.send_header('Content-type', 'text/plain')
		self.end_headers()
		self.wfile.write("Ok\n".encode('utf-8'))
		return

try:
	server_address = ('127.0.0.1', 8080)
	httpd = HTTPServer(server_address, MyHTTPRequestHandler)
	httpd.serve_forever()

except KeyboardInterrupt:
	httpd.socket.close()
