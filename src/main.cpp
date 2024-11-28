// Libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED pin definitions
#define OLED_CLK      23
#define OLED_MOSI     22
#define OLED_RST      4
#define OLED_DC       5
#define OLED_CS       12

// Buttons
#define LButton       18
#define RButton       19

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

// int board[3][3] = {{1, -1, 1}, {1, 1, 0}, {-1, 0, -1}};
int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int currentRow;
int currentCol;
const int blinkTime = 250;
int player = 1;
int winner = 0;

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
          break;
        case 1:
          colString = " X ";
          rowString = rowString + colString;
          break;
        case -1:
          colString = " O ";
          rowString = rowString + colString;
          break;
      }
      if (column < 2) {
        rowString = rowString + "|";
      }
    } 
    display.setCursor(0, row*10); // Set cursor position
    display.println(rowString);
  }
  display.display(); // Update the display with the buffer content
}


void displayBoardSerial() {
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
    Serial.println();
  }
  Serial.println();
  Serial.println("--------------------");
}

void blinkBoard(bool tokenBlink) {
  if (tokenBlink == true) {
    board[currentRow][currentCol] = player;
  } else {
    board[currentRow][currentCol] = 0;
  }
  displayBoard();
}

int checkForInput() {
  if (digitalRead(LButton) == HIGH) {
    return 1;
  } else if (digitalRead(RButton) == HIGH) {
    return 2;
  } else {
    return 0;
  }
}

void selectSquare() {
  int availableSquares = 0;
  bool squareSelected = false;
  while (squareSelected == false) { // infinite loop which I need to fix later
    for (int row=0;row<3;row++) { //rows
      for (int column=0;column<3;column++) { //columns
        if (board[row][column] == 0) {
          Serial.println(row);
          Serial.println(column);
          currentRow = row;
          currentCol = column;
          int action = 0; // 0 = nothing, 1 = select, 2 = next square
          int nextBlink = millis() + blinkTime;
          bool tokenBlink = true;
          while (action == 0) {
            if (millis() > nextBlink) {
              blinkBoard(tokenBlink);
              nextBlink = millis() + blinkTime;
              tokenBlink = !tokenBlink;
            }
            action = checkForInput();
          }
          delay(300);
          Serial.print("action: ");
          Serial.println(action);
          if (action == 1) {
            board[currentRow][currentCol] = player;
            return;
          } else {
             board[currentRow][currentCol] = 0;
          }
        }
      }
    }
  }
}


bool checkTermination2D() {
  for (int i=0;i<3;i++) {
    if ((board[i][0] == board[i][1]) && (board[i][1] == board[i][2]) && (board[i][0] != 0)) { // check rows
      winner = board[i][0];
      return true;
    }
    if ((board[0][i] == board[1][i]) && (board[1][i] == board[2][i]) && (board[0][i] != 0)) { // check columns
      winner = board[0][i];
      return true;
    }
  }
  // Check diagonals
  if (board[1][1] != 0) {
    if ((board[0][0] == board[1][1]) && (board[1][1] == board[2][2])) {
      winner = board[1][1];
      return true;
    }
    if ((board[2][0] == board[1][1]) && (board[1][1] == board[0][2])) {
      winner = board[1][1];
      return true;
    }
  }
  // Check draw
  for (int row=0;row<3;row++) {
    for (int column=0;column<3;column++) {
      if (board[row][column] == 0) {
        return false;
      }
    }
  }
  return true;
}

void printWinners() {
  if (winner == 0) {
    Serial.println("DRAW!");
  } else {
    Serial.print("The winner is player ");
    if (winner == 1) {
      Serial.print("X");
    } else {
      Serial.print("O");
    }
    Serial.println("!");
  }
  delay(500);
}

void resetGame() {
  for (int row=0;row<3;row++) {
    for (int column=0;column<3;column++) {
      board[row][column] = 0;
    }
  }
}

void setup()   {
  Serial.begin(9600); // Initialise Serial for later debugging
  delay(100);
  Serial.println("PLZ WORK");
  delay(1000);

  pinMode(LButton, INPUT);
  pinMode(RButton, INPUT);

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
  while (checkTermination2D() == false) {
      selectSquare();
      player = -player;
  }
  printWinners();
  delay(500);
  resetGame();
}