import paho.mqtt.client as mqtt
import time

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "home/orangepi/temp"

def get_cpu_temp():
    try:
        with open('/sys/class/thermal/thermal_zone0/temp', 'r') as f:
            return round(int(f.read().strip()) / 1000.0, 1)
    except Exception:
        return None

client = mqtt.Client("OrangePi_Temp_Publisher")
client.connect(MQTT_BROKER, MQTT_PORT, 60)
client.loop_start()

try:
    while True:
        temp = get_cpu_temp()
        if temp is not None:
            client.publish(MQTT_TOPIC, payload=str(temp))
            print(f"Опубликована температура: {temp} °C")
        time.sleep(5)
except KeyboardInterrupt:
    pass
finally:
    client.loop_stop()
    client.disconnect()
