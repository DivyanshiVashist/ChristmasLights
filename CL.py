import paho.mqtt.client as mqtt
import json 
import time
import datetime
import tweepy

                                          
# ----- INSERT YOUR ACCOUNT INFORMATION BELOW ---------------
CONSUMER_KEY = "INSERT YOUR CONSUMER KEY HERE" 
CONSUMER_SECRET = "INSERT YOUR CONSUMER SECRET KEY HERE"
ACCESS_KEY= "INSERT YOUR ACCESS KEY HERE"
ACCESS_SECRET = "INSERT YOUR ACCESS SECRET KEY HERE"


auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)  
auth.set_access_token(ACCESS_KEY, ACCESS_SECRET) 
api = tweepy.API(auth)

start= time.time()
lastpat = 0

def on_connect(client, userdata, flags, rc):
	client.subscribe("ChristmasLights")


def getpost ():	
	global lastpatcd 
	direct_message = api.direct_messages(count=1)
	strdm= str(direct_message)
	position= strdm.index('text=u')
	command = (strdm[int(position+7):int(position+7+25)])
	
	if "lightning" in command:
		command= "lightning"
	if"alternating" in command:
		command = "alternating"
	if "onoff" in command:
		command = "onoff"
	if "fade" in command:
		command = "fade"
	if "fast" in command:
		command = "fade"
	if "skip" in command:
		command= "skip"
	if "dance" in command:
		command= "dance"
	if "pat" in command:
		command = "pat"
	
	client.connect("winter.ceit.uq.edu.au", 1883, 60)
	if lastpat != command:
		client.publish("ChristmasLights", str(command) ) 
		print command
		lastpat = command

client = mqtt.Client()
client.on_connect = on_connect
getpost() 
						
while 1: 
	getpost()
	time.sleep(15)
