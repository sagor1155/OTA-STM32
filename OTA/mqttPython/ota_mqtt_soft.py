import time
import os
import paho.mqtt.client as mqtt


broker = 'm11.cloudmqtt.com'
port   =  10902
user   = 'fdffditt'
key    = 'gJpCf2V1O4WK'
remain_session = 360
topic_sub = "541610203040/feedback"
topic_pub = "541610203040/ihexdata"

fp = open('example.hex')
client = mqtt.Client()


def on_connect(client, obj, flags, rc):
        if rc==0:
                print ("Subscriber Connection status code: " + str(rc) + " | Connection status: successful")
        elif rc==1:
                print ("Subscriber Connection status code: " + str(rc)+" | Connection status: Connection refused")

def on_subscribe(client, obj, mid, granted_qos):
        print("Subscribed: " + str(mid) + " " + str(granted_qos) + "data" + str(obj))

def on_message(client, obj, msg):
        print (str(msg.payload))
        print("Received message from topic: "+msg.topic+" | QoS: "+str(msg.qos)+" | Data Received: "+str(msg.payload))


def mqtt_connect():
        client.on_connect   = on_connect
        client.on_subscribe = on_subscribe
        client.on_message   = on_message

        client.username_pw_set(user, key)     ##set user name & password
        print ("Connecteing To Broker ..........")
        rt = client.connect(broker, port, remain_session)
        return rt

def mqtt_publish(payload):
        for i in range(0, 3, 1):
                print ("Publishing Data: " + str(payload))
                client.publish(topic_pub, payload, qos=1, retain=False)
                time.sleep(1)


if __name__ == "__main__":
        con_status = False
        if  mqtt_connect()==0:
                print ("MQTT Broker Connected")
                con_status = True
        else:
                print ("Connection Failled !!!")
                con_status = False
        client.subscribe(topic_sub, 1)
        client.loop_start()
        try:
                while(True):
                        if con_status==True:
                        		line = fp.readline()
    							if not line:
    							    	break
    							try:
    							    	byte_string = line.split(':', 1)
    							    	# print(rd)
    							    	mqtt_publish(byte_string[1])
    							    	time.sleep(0.1)

    							except Exception as e:
    							    	print (e)
                                
                        else:
                                print ("MQTT Connection Failled !!!")

        except KeyboardInterrupt:
                client.disconnect()

