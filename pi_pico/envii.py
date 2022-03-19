# Maker Pi RP2040 (connected at GROVE2) or
# Pi Pico (connected at pin 6 and 7)
#   with M5Stack ENV II Unit
import time
import board
import busio
import adafruit_bmp280
import adafruit_sht31d

if 'board_id' in dir(board) and board.board_id == 'raspberry_pi_pico':
    i2c = busio.I2C(board.GP5, board.GP4)
else:
    i2c = busio.I2C(board.GP3, board.GP2)
sht30 = adafruit_sht31d.SHT31D(i2c)
bmp280 = adafruit_bmp280.Adafruit_BMP280_I2C(i2c, 0x76)
bmp280.sea_level_pressure = 1008.5 # arbitrary choice

while True:
    print(f"Temperature: {sht30.temperature} {bmp280.temperature} Humidity: {sht30.relative_humidity} Pressure: {bmp280.pressure} Altitude: {bmp280.altitude}")
    time.sleep(2)
