#!/usr/bin/env python3
# Copyright 2016 Tero Karvinen & Kimmo Karvinen http://BotBook.com
"BotBook.com Python IoT server. "

import serial
import dbsettings
from time import sleep
from urllib.request import urlopen
from urllib.error import HTTPError
import json
import requests
import emailSend

def serReadLine(ser):
	s = ser.readline(500)
	s = s.decode('ASCII')
	assert type(s)==str

	if len(s)<=1:
		return None
	if not s.endswith('\n'):
		return 'ERROR: Serial: Unknown command "%s"' % s
	s = s.replace('\n', '');

	return s

def isFoundInCards(cardnumber):
	data = dbsettings.getCardnumbers()
	found = "Access Denied!"
	for i in data:
		if(data[i] == int(cardnumber)):
			found = "Access granted!"
			break

	return found

def waitForResponse():
	i = 0
	while True:
		print(i)
		url= "http://api.botbook.com/last/apikey"
		body=requests.get(url)
		sleep(1)
		if body.text[:3] == "2.0":
			body=requests.get("http://api.botbook.com/add/apikey/?x=1")		
			return 'Open Door'
		if(i >= 40):
			return 'Verification timeout'
		i = i + 1


def main():
	ser = serial.Serial(port="/dev/ttyACM0", baudrate=115200, timeout=1)

	while True:
		s = serReadLine(ser)
		if s: 
			if s.startswith('KOODI'):
				cardnumber=s[6:]
				print('Card scanned    ',cardnumber)
				message=isFoundInCards(cardnumber) 
				if message == "Access granted!":					
					emailSend.sendMail()
					print("email sent, waiting for user")
					message=waitForResponse()
				for i in range(16-len(message)):
					message=message+" "
					#workaround for 16 letter lcd-display
				print(message)
				message=message.encode("ASCII")						
				ser.write(message)
			else:
				msg='ERROR: Python: unknown command "%s"' % s.replace('\n', '')			
				ser.write(s.encode('ASCII'))
		sleep(0.2)

if __name__ == '__main__':
	main()






