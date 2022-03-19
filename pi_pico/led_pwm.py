"""Example for Pico. Blinks the built-in LED."""
import time
import board
import digitalio
import pwmio

#led = digitalio.DigitalInOut(board.LED)
#led.direction = digitalio.Direction.OUTPUT

ledpwm = pwmio.PWMOut(board.LED, frequency=660, duty_cycle=0, variable_frequency=True)
upward = True
start = 3
i = start
div = 0.1

while True:
    if upward:
        if i > 14:
            upward = False
            i -= div
        else:
            i += div
    else:
        if i < start:
            upward = True
            i += div
        else:
            i -= div
    ledpwm.duty_cycle = int(pow(2, i))
    time.sleep(0.01)
