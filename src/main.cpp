// Libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED pin definitions
#define OLED_CLK      23
#define OLED_MOSI     22
#define OLED_RST      1
#define OLED_DC       3
#define OLED_CS       12


// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

int board[3][3] = {{1, -1, 1}, {1, 1, 0}, {-1, 0, -1}};

void displayExample() {
  display.clearDisplay();
  String stringOne   = " X | O | X ";
  String stringTwo   = " X | O | X ";
  String stringThree = " O | X | O ";

  display.setTextSize(1); // Set text size
  display.setTextColor(SH110X_WHITE); // Set text color

  display.setCursor(0, 0); // Set cursor position
  display.println(stringOne); // Print message

  display.setCursor(0, 10); // Set cursor position
  display.println(stringTwo); // Print message

  display.setCursor(0, 20); // Set cursor position
  display.println(stringThree); // Print message

  display.display(); // Update the display with the buffer content
}

void displayBoard() {
  display.clearDisplay();
  display.setTextSize(1); // Set text size
  display.setTextColor(SH110X_WHITE); // Set text color
  for (int row=0;row<3;row++) { //rows
    String rowString = "";
    for (int column=0;column<3;column++) { //columns
      String colString = "";
      switch (board[row][column]) {
        case 0:
          colString = "   ";
          rowString = rowString + colString;
          Serial.print(colString);
          break;
        case 1:
          colString = " X ";
          rowString = rowString + colString;
          Serial.print(colString);
          break;
        case -1:
          colString = " O ";
          rowString = rowString + colString;
          Serial.print(colString);
          break;
      }
      if (column < 2) {
        rowString = rowString + "|";
        Serial.print("|");
      }
    } 
    display.setCursor(0, row*10); // Set cursor position
    display.println(rowString);
    Serial.println();
  }
  display.display(); // Update the display with the buffer content
  Serial.println();
  Serial.println("--------------------");
}

void setup()   {
  Serial.begin(9600); // Initialise Serial for later debugging
  delay(100);
  Serial.println("PLZ WORK");
  delay(1000);

  // Start OLED
  display.begin(0, true); // we dont use the i2c address but we will reset!

  display.setRotation(2); // Ensures the display is not upside down
  display.clearDisplay(); // Clear the display buffer

  display.setTextSize(1); // Set text size
  display.setTextColor(SH110X_WHITE); // Set text color
  display.setCursor(0, 0); // Set cursor position
  display.println("Hello, World!"); // Print message
  display.display(); // Update the display with the buffer content
  display.clearDisplay(); // Clear the display buffer

  delay(1000);
}


void loop() {
  displayExample();
  delay(2000);
  displayBoard();
  delay(2000);
  Serial.println("Hello Worldddd");
}

