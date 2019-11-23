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

fp = open('ClockModuleStudy.hex')
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
    print ("Publishing Data: " + str(payload))
    client.publish(topic_pub, payload, qos=1, retain=True)
    

if __name__ == "__main__":
    con_status = False
    if  mqtt_connect()==0:
        print ("MQTT Broker Connected")
        con_status = True
    else:
        print ("Connection Failled !!!")
        con_status = False
    #client.subscribe(topic_sub, 1)
    client.loop_start()
    time.sleep(1)    
    try:
        while(True):
            if con_status==True:
                msg = ""
                line = fp.readline()
                #for i in range(0, 1, 1):
                #    line = fp.readline()
                #    msg = msg + line
                #    #byte_string = line.split('\n', 1)
                #    #msg = msg + byte_string[0]

                if not line:
                    #mqtt_publish(msg)
                    break
                try:
                    #byte_string = line.split(':', 1)
                    mqtt_publish(line)
                    #msg = ""
                    time.sleep(1)

                except Exception as e:
                    print (e)
        
            else:
                print ("MQTT Connection Failled !!!")
                mqtt_connect()

    except KeyboardInterrupt:
        client.disconnect()

