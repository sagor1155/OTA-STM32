import paho.mqtt.client as mqtt
import time
import os 


broker = 'm11.cloudmqtt.com'
port   =  10902
user   = 'fdffditt'
key    = 'gJpCf2V1O4WK'

# broker = "io.adafruit.com"
# port = 1883
# user = "sagor"
# key = "2c1f1941c5aa4903a85ddc8e47197673"

topic = "sagor/feeds/newTopic"
subTopic = "sagor/feeds/newTopic"
remain_session = 360
data = "0000/00/00/00/00"

client = mqtt.Client()     ##create client object


#called while client tries to establish connection with the server
def on_connect(client, obj, flags, rc):
        if rc==0:
                print ("Subscriber Connection status code: " + str(rc) + " | Connection status: successful")
        elif rc==1:
                print ("Subscriber Connection status code: " + str(rc)+" | Connection status: Connection refused")

#called when a topic is successfully subscribed to
def on_subscribe(client, obj, mid, granted_qos):
        print("Subscribed: " + str(mid) + " " + str(granted_qos) + "data" + str(obj))

#called when a message is received by a topic
def on_message(client, obj, msg):
        print (str(msg.payload))
        print("Received message from topic: "+msg.topic+" | QoS: "+str(msg.qos)+" | Data Received: "+str(msg.payload))



def mqtt_connect():
        client.on_connect = on_connect
        client.on_subscribe = on_subscribe
        client.on_message = on_message

        client.username_pw_set(user, key)     ##set user name & password
        print ("Connecteing To Broker ..........")
        ##connect to broker
        rt = client.connect(broker, port, remain_session)
        return rt

def mqtt_publish(payload):
        for i in range(0, 3, 1):
                print ("Publishing Data: " + str(payload))
                client.publish(topic, payload, qos=1, retain=False)
                time.sleep(1)


if __name__ == "__main__":
        conack = False
        if  mqtt_connect()==0:
                print ("MQTT Broker Connected")
                conack = True
        else:
                print ("Connection Failled !!!")
                conack = False
        client.subscribe(subTopic, 1)
        client.loop_start()
        try:
                while(True):
                        if conack==True:
                                data = input("Enter Alarm time(Year/Month/Day/Hour/Minute): ")
                                mqtt_publish(data)
                        else:
                                print ("MQTT Connection Failled !!!")


        except KeyboardInterrupt:
                client.disconnect()


                

                
