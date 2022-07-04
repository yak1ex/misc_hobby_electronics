#if 0 /* LED blink */
const int LED = 26;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}
#endif

#if 0 /* FRAM INFO */
#include <SPI.h>
#include "Adafruit_FRAM_SPI.h"

/* Example code to interrogate Adafruit SPI FRAM breakout for address size and storage capacity */

/* NOTE: This sketch will overwrite data already on the FRAM breakout */

uint8_t FRAM_CS = 4;
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);  // use hardware SPI

uint8_t FRAM_SCK = 13;
uint8_t FRAM_MISO = 12;
uint8_t FRAM_MOSI = 11;
//Or use software SPI, any pins!
//Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);

uint8_t           addrSizeInBytes = 2; //Default to address size of two bytes
uint32_t          memSize;

int32_t readBack(uint32_t addr, int32_t data) {
  int32_t check = !data;
  int32_t wrapCheck, backup;
  fram.read(addr, (uint8_t*)&backup, sizeof(int32_t));
  fram.writeEnable(true);
  fram.write(addr, (uint8_t*)&data, sizeof(int32_t));
  fram.writeEnable(false);
  fram.read(addr, (uint8_t*)&check, sizeof(int32_t));
  fram.read(0, (uint8_t*)&wrapCheck, sizeof(int32_t));
  fram.writeEnable(true);
  fram.write(addr, (uint8_t*)&backup, sizeof(int32_t));
  fram.writeEnable(false);
  // Check for warparound, address 0 will work anyway
  if (wrapCheck==check)
    check = 0;
  return check;
}

bool testAddrSize(uint8_t addrSize) {
  fram.setAddressSize(addrSize);
  if (readBack(4, 0xbeefbead) == 0xbeefbead)
    return true;
  return false;
}


void setup(void) {
  Serial.begin(9600);

  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  
  if (fram.begin(addrSizeInBytes)) {
    Serial.println("Found SPI FRAM");
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
    while (1);
  }

  if (testAddrSize(2))
    addrSizeInBytes = 2;
  else if (testAddrSize(3))
    addrSizeInBytes = 3;
  else if (testAddrSize(4))
    addrSizeInBytes = 4;
  else {
    Serial.println("SPI FRAM can not be read/written with any address size\r\n");
    while (1);
  }
  
  memSize = 0;
  while (readBack(memSize, memSize) == memSize) {
    memSize += 256;
    //Serial.print("Block: #"); Serial.println(memSize/256);
  }
  
  Serial.print("SPI FRAM address size is ");
  Serial.print(addrSizeInBytes);
  Serial.println(" bytes.");
  Serial.println("SPI FRAM capacity appears to be..");
  Serial.print(memSize); Serial.println(" bytes");
  Serial.print(memSize/0x400); Serial.println(" kilobytes");
  Serial.print((memSize*8)/0x400); Serial.println(" kilobits");
  if (memSize >= (0x100000/8)) {
    Serial.print((memSize*8)/0x100000); Serial.println(" megabits");
  }
}

void loop(void) {

}
#endif

#if 0
const int LED = 26;

#include <SPI.h>
#include "Adafruit_FRAM_SPI.h"

/* Example code for the Adafruit SPI FRAM breakout */
uint8_t FRAM_CS = 4;

Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);  // use hardware SPI

//uint8_t FRAM_SCK= 13;
//uint8_t FRAM_MISO = 12;
//uint8_t FRAM_MOSI = 11;
//Or use software SPI, any pins!
//Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);

uint16_t          addr = 0;

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  
  if (fram.begin()) {
    Serial.println("Found SPI FRAM");
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
    while (1);
  }
  
  // Read the first byte
  uint8_t test = fram.read8(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");

  // Test write ++
  fram.writeEnable(true);
  fram.write8(0x0, test+1);
  fram.writeEnable(false);

  fram.writeEnable(true);
  fram.write(0x1, (uint8_t *)"FTW!", 5);
  fram.writeEnable(false);

  // dump the ~~entire 8K~~ first 256 bytes of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 256; a++) {
    value = fram.read8(a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  }

  pinMode(LED, OUTPUT);
}

void loop(void) {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}
#endif

#include "PCF8574.h"

PCF8574 pcf8574(0x20);

unsigned long lastTime;
void setup(void) {
    Serial.begin(115200);
    delay(1000);
    Serial.println("INIT");

    pcf8574.pinMode(P4, OUTPUT); // 1:SD(on TFT) 0:CAM
    // IO12: SD_MISO / D4
    // IO13: SD_MOSI / VSYNC
    // IO14: SD_SCK  / PCLK
    // IO15: SD_CS   / XCLK
    pcf8574.pinMode(P5, INPUT); // SW
    Serial.print("Init pcf8574...");
    if(pcf8574.begin()) {
        Serial.println("OK");
    } else {
        Serial.println("KO");
    }
}

void loop(void) {
    uint8_t val = pcf8574.digitalRead(P5);
    if(val) {
        lastTime = millis();
    }
    if(millis() - lastTime < 1000) {
        Serial.print("P5 ")
        Serial.println(val);
    }
    int aval = analogRead(33);
    Serial.print("A5(IO33) ")
    Serial.println(aval);
    delay(100);
}

// EXPANDER(0x20)
// SDA(SDA(IO21))
// SCL(SCL(IO22))

// TFT
// CS(IO5)
// SCK(IO18)
// MISO(IO19)
// MOSI(IO23)
// DC(IO27)

// SD_MISO(IO12)
// SD_MOSI(IO13)
// SD_SCK(IO14)
// SD_CS(IO15)

// CAM(0x21)
// SDA(SDA(IO21))
// SCL(SCL(IO22))
// VSYNC(IO13)
// PCLK(IO14)
// XCLK(IO15)
// D7(IO17)
// D6(IO16)
// D5(IO2)
// D4(IO12)
// D3(IO35)
// D2(IO34)
// D1(SENSOR_VN(IO39))
// D0(SENSOR_VP(IO36))

// FRAM1
// CS(IO4)
// SCK(IO18)
// SO(IO19)
// SI(IO23)
// FRAM2
// CS(IO32)
// SCK(IO18)
// SO(IO19)
// SI(IO23)

// LED
// IO26

// MIC
// IO33
