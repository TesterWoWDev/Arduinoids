#include <Arduino.h>
#include <U8x8lib.h>

int gameLoopTimer = 100;
int gameOverTimer = 10000;
int enemyMoves = 1;

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
int pX = 0;
int pY = 0;
const int rows = 8;
int eXs[rows];
int eYs[rows];
int score = 0;
bool greenBtn = 1; //0 means pressed
bool redBtn = 1;
bool chosen = 0;
bool firstRun = 0;
int ledPin = 4;
int buzzPin = 5;
int resetbtn = 6;
int button1 = 7; //set the button1 pin//plug double button to D7
int button2 = 8; //set the button2 pin
int highScore = 0;

void setup(void) {
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin();
  u8x8.setFlipMode(1);//upside down
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(resetbtn, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  reset();
}

void reset() {
  u8x8.clear();
  digitalWrite(ledPin, LOW);
  noTone(buzzPin);
  enemyMoves = 1;
  firstRun = 1;
  pX = 0;
  pY = 0;
  chosen = 0;
  greenBtn = 1;
  redBtn = 1;
  score = 0;
  for (int i = 0; i < rows; i++) {
    eXs[i] = 15;
    eYs[i] = i;
  }
}

void gameLoop() {

  if (greenBtn == 0 && pY < (rows - 1)) {
    u8x8.clearLine(pY);
    pY++;
  }
  if (redBtn == 0 && pY > 0) {
    u8x8.clearLine(pY);
    pY--;
  }
  //move enemy + draw all
  for (int i = 0; i < enemyMoves; i++) {
    int cur = random(rows);
    u8x8.clearLine(cur);
    eXs[cur]--;
    drawEntities();
  }
  score = score + enemyMoves;
  delay(gameLoopTimer);

}

void drawEntities() {
  //  u8x8.clearDisplay();
  //  //draw player
  u8x8.setCursor(pX, pY);
  u8x8.print("O");
  //draw enemies
  for (int i = 0; i < rows; i++) {
    if (eXs[i] < 0) {
      eXs[i] = 15;
    }
    u8x8.setCursor(eXs[i], eYs[i]);
    u8x8.print("<");
    checkLose(i);
  }
}

void checkLose(int i) {
  if (eXs[i] == pX && eYs[i] == pY) {
    if (score > highScore) {
      highScore = score;
    }
    u8x8.clear();
    u8x8.setCursor(0, 1);
    u8x8.print("    YOU LOST");
    u8x8.setCursor(0, 4);
    u8x8.print("Score:");
    u8x8.print(score);
    u8x8.setCursor(0, 6);
    u8x8.print("High Score:");
    u8x8.print(highScore);
    //tone(buzzPin, 294);
    digitalWrite(ledPin, HIGH);
    delay(gameOverTimer);
    reset();
  }
}
void randomXs() {
  for (int i = 0; i < rows; i++) {
    eXs[i] = random(10) + 6;
  }
}
void loop(void) {
  if (firstRun == 1) {
    u8x8.clear();
    firstRun = 0;
  }
  //use D6 button as reset
  //use joystick to move(somehow)
  if (digitalRead(resetbtn) == 1) {
    reset();
  }
  greenBtn = digitalRead(button1);
  redBtn = digitalRead(button2);
  if (chosen == 1) {
    gameLoop();
  }
  else {
    u8x8.setCursor(0, 0);
    u8x8.print("Press green btn");
    u8x8.setCursor(0, 1);
    u8x8.print("for normal play");

    u8x8.setCursor(0, 3);
    u8x8.print("Press red btn to");
    u8x8.setCursor(0, 4);
    u8x8.print("increase diff");
    u8x8.setCursor(0, 6);
    u8x8.clearLine(6);
    u8x8.print("current diff:");
    u8x8.print(enemyMoves);
    if (greenBtn == 0) {
      chosen = 1;
      firstRun = 1;
    } else if (redBtn == 0) {
      enemyMoves++;
    }
    delay(gameLoopTimer);
  }
}
