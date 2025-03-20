import lgpio
import paho.mqtt.client as mqtt

LED_PIN = 17  # GPIO pin

# Opsætning af GPIO
chip = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(chip, LED_PIN)

# Callback-funktion til at styre LED via MQTT
def on_message(client, userdata, msg):
    message = msg.payload.decode()
    print(f"Modtaget besked: {message}")
    if message == "ON":
        lgpio.gpio_write(chip, LED_PIN, 1)
    elif message == "OFF":
        lgpio.gpio_write(chip, LED_PIN, 0)

client = mqtt.Client()
client.connect("localhost", 1880, 60)
client.subscribe("led/control")

client.on_message = on_message
client.loop_forever()

