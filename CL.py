from twython import Twython

import paho.mqtt.client as mqtt
import json 
import time
import datetime



CONSUMER_KEY = 'lVhJVwewWe3idmNl92XgI8COT'
CONSUMER_SECRET = 'vyzzKFwmdLDszuSx0q7AaqNbQalX8IbAwLXXdxX1LSzR69ziRN'


ACCESS_KEY= '2918024730-IzpIM8nhjj06hqBevov4bQ2QLwH5K8YdwQZH0X8'
ACCESS_SECRET = 'aBctWjjijY7hYM4QKfnlo3SGbyHGfjSTxdfHA0o7xWf1i'



twitter = Twython(CONSUMER_KEY, CONSUMER_SECRET,
                  ACCESS_KEY, ACCESS_SECRET)

twitter.verify_credentials()

twitter.get_home_timeline()

start= time.time()
lastpat = 0

def on_connect(client, userdata, flags, rc):

	client.subscribe("/ChristmasLights/")




def getpost ():	
	global lastpat
	direct_message = twitter.get_direct_messages(since_id=240136858829479935)


	for message in direct_messages: 
		command = message['text']
		
	
	#user_timeline = twitter.get_user_timeline(screen_name="TheThirstyPlant",count=1)
	#for tweet in user_timeline:
	#	rectwt = tweet['text']
		#client.connect("winter.ceit.uq.edu.au", 1883, 60)
		#if lastpat != rectwt:
		#	client.publish("/ChristmasLights/", str(rectwt) ) 
		#	print rectwt

#			lastpat = rectwt

		client.connect("winter.ceit.uq.edu.au", 1883, 60)
		if lastpat != command:
			client.publish("/ChristmasLights/", str(command) ) 
			print command

			lastpat = command	
	
	
client = mqtt.Client()
client.on_connect = on_connect
getpost()

while 1: 
	getpost()
	time.sleep(15)
