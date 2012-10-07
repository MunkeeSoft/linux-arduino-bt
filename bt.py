#!/usr/bin/python

# Require required stuff
import sys, argparse

# Check for arguments
parser = argparse.ArgumentParser(description='Query a Bluetooth node and (optionally) post data to Cosm.', epilog="If you do not include all the Cosm arguments, data will only be printed.")
parser.add_argument('-k', type=str, help="Cosm API key", metavar="Key")
parser.add_argument('-f', type=str, help="Cosm feed ID", metavar="Feed ID")
parser.add_argument('-d', type=str, help="Cosm feed datapoint ID", metavar="Datapoint ID")
parser.add_argument('-q', help="Quiet. Only display response output.", action="store_true", default=False)
parser.add_argument('-qq', help="Really quiet.  Do not display any output", action="store_true", default=False)
parser.add_argument('mac', type=str, help="MAC address of Bluetooth node", metavar="MAC")
parser.add_argument('cmd', type=str, help="Command to send to node", metavar="Command")
args = parser.parse_args()

# Assign variables
mac = args.mac
cmd = args.cmd

# Check if we're posting to Cosm
if args.k!=None and args.f !=None and args.d!=None:
	c = True
else:
	c = False

# Make things extra quiet
if args.qq==True:
	args.q = True


# More requirements
from bluetooth import *
import time, json
import Cosm


# Connect to Bluetooth device
sock = BluetoothSocket(RFCOMM)
sock.connect((mac, 1))
if args.q==False:
	print "Connected!"


# Send command
if args.q==False:
	print "Sent: ", cmd
sock.send(cmd)


# Sleep to give Arduino time to process
time.sleep(2)

# Get the response
response = json.loads(sock.recv(1024))
if args.q==False:
	print "Received: ", response['value']
elif args.q==True and args.qq==False:
	print response['value']

# Initialize Cosm
if c==True:
	if args.q==False:
		print "Posting to Cosm"

	C = Cosm.CosmFeedUpdate(args.f, args.k)

	# Add data and update
	C.addDatapoint(args.d, response['value'])
	C.buildUpdate()
	C.sendUpdate()

# Disconnect
if args.q==False:
	print "Disconnecting"

sock.close()
