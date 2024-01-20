# This file is executed on every boot (including wake-boot from deepsleep)
import asyncio
import network
import time
from machine import Pin

from microdot import Microdot

import config

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(config.WIFI_SSID, config.WIFI_PASSWORD)

while not station.isconnected():
    time.sleep(1)

print(station.ifconfig())

app = Microdot()
led = Pin(2, Pin.OUT)
led.on()

@app.route('/on')
async def on(request):
    led.on()
    return 'OK'

@app.route('/off')
async def off(request):
    led.off()
    return 'OK'

async def oneshot_off():
    led.off()
    await asyncio.sleep(1)
    led.on()

@app.route('/reset')
async def oneshot(request):
    asyncio.create_task(oneshot_off())
    return 'OK'

app.run(port=80)