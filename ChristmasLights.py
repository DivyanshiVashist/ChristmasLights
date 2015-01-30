import paho.mqtt.client as mqtt
import json 
import time
import datetime
from tweepy.streaming import StreamListener
from tweepy import Stream
import tweepy


lastpat = 0                                          

# ----- INSERT YOUR ACCOUNT INFORMATION BELOW ---------------
CONSUMER_KEY = "INSERT YOUR OWN CONSUMER KEY IN HERE"
CONSUMER_SECRET = "INSERT YOUR OWN CONSUMER SECRET KEY IN HERE"
ACCESS_KEY= "INSERT YOUR ACCESS KEY IN HERE"
ACCESS_SECRET = "INSERT YOUR SECRET ACCESS KEY IN HERE"

class StdOutListener(StreamListener):
    """ A listener handles tweets are the received from the stream.
    This is a basic listener that just prints received tweets to stdout.
    """


    def on_data(self, data):
        global lastpat
        
        print(data)
    	command = str(data)
	
    	if "lightning" in command:
    		command= "lightning"
    	if"alternating" in command:
    		command = "alternating"
    	if "onoff" in command:
    		command = "onoff"
    	if "fade" in command:
    		command = "fade"
    	if "fast" in command:
    		command = "fast"
    	if "skip" in command:
    		command= "skip"
    	if "dance" in command:
    		command= "dance"
    	if "jingle" in command:
    		command = "jingle"
	
    	if lastpat != command:
    		client.publish("ChristmasLights", command) 
#    		print command
    		lastpat = command
        
        return True

    def on_error(self, status):
#        print(status)
        client.publish("ChristmasLights/log", bytearray(status))


def on_connect(client, userdata, flags, rc):
	client.subscribe("ChristmasLights")

client = mqtt.Client()
client.on_connect = on_connect
client.connect("winter.ceit.uq.edu.au", 1883, 60)
client.loop_start()

l = StdOutListener()
auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)  
auth.set_access_token(ACCESS_KEY, ACCESS_SECRET) 

stream = Stream(auth, l)
stream.filter(track=['TheThirstyPlant'])
