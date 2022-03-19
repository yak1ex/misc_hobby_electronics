import time
import board
import neopixel

print("Loop starting!")
for i in range(10):
    print(f"Loop number: {i}")
print("Loop finished!")

"""CircuitPython Essentials Pin Map Script"""
import microcontroller
import board

board_pins = []
for pin in dir(microcontroller.pin):
    if isinstance(getattr(microcontroller.pin, pin), microcontroller.Pin):
        pins = []
        for alias in dir(board):
            if getattr(board, alias) is getattr(microcontroller.pin, pin):
                pins.append("board.{}".format(alias))
        if len(pins) > 0:
            board_pins.append(" ".join(pins))
for pins in sorted(board_pins):
    print(pins)


pixels = neopixel.NeoPixel(board.NEOPIXEL, 2)

while True:
    pixels[0] = (255, 0, 0)
    pixels[1] = (255, 255, 0)
    time.sleep(0.5)
    pixels.fill((0, 0, 0))
    time.sleep(0.5)
