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

#if 0
#include "PCF8574.h"

PCF8574 pcf8574(0x20);

unsigned long lastTime;
void setup(void) {
    Serial.begin(115200);
    Wire.begin(21,22);
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
        Serial.print("P5 ");
        Serial.println(val);
    }
    int aval = analogRead(33);
//    Serial.print("A5(IO33) ");
//    Serial.println(aval);
    delay(10);
}
#endif

#if 0
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(5 /*TFT_CS*/, 27 /*TFT_DC*/, 23 /*TFT_MOSI*/, 18/*TFT_CLK*/, -1 /*TFT_RST*/, 12 /*TFT_MISO*/);

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
 
  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  Serial.println("Benchmark                Time (microseconds)");
  delay(10);
  Serial.print("Screen fill              ");
  Serial.println(testFillScreen());
  delay(500);

  Serial.print("Text                     ");
  Serial.println(testText());
  delay(3000);

  Serial.print("Lines                    ");
  Serial.println(testLines(ILI9341_CYAN));
  delay(500);

  Serial.print("Horiz/Vert Lines         ");
  Serial.println(testFastLines(ILI9341_RED, ILI9341_BLUE));
  delay(500);

  Serial.print("Rectangles (outline)     ");
  Serial.println(testRects(ILI9341_GREEN));
  delay(500);

  Serial.print("Rectangles (filled)      ");
  Serial.println(testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(500);

  Serial.print("Circles (filled)         ");
  Serial.println(testFilledCircles(10, ILI9341_MAGENTA));

  Serial.print("Circles (outline)        ");
  Serial.println(testCircles(10, ILI9341_WHITE));
  delay(500);

  Serial.print("Triangles (outline)      ");
  Serial.println(testTriangles());
  delay(500);

  Serial.print("Triangles (filled)       ");
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print("Rounded rects (outline)  ");
  Serial.println(testRoundRects());
  delay(500);

  Serial.print("Rounded rects (filled)   ");
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println("Done!");

}


void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(1000);
  }
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(i, i, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i*10, i*10));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i*10, i*10, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}
#endif

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(5 /*TFT_CS*/, 27 /*TFT_DC*/, 23 /*TFT_MOSI*/, 18/*TFT_CLK*/, -1 /*TFT_RST*/, 12 /*TFT_MISO*/);

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
 
  tft.begin();
  tft.setRotation(1);
}

void loop() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.println("Hello World!");
  delay(1000);
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
