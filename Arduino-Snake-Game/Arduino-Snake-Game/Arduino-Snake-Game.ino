#include <FastLED.h>

#define DATA_PIN 6        
#define NUM_LEDS 64       
#define JOYSTICK_X A0     
#define JOYSTICK_Y A1     
#define JOYSTICK_SW 8     
#define LED_INDEX 12      

CRGB leds[NUM_LEDS];
int snakeLength = 3;       
int snakeX[64];            
int snakeY[64];            
int appleX, appleY;        
int direction = 1;        
int speed = 350;     
unsigned long lastMoveTime; 
bool gameOverFlag = false; 
int applesEaten = 0;       
bool restartGame = false;  

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.show();

  randomSeed(analogRead(0)); 

  
  resetGame();

  lastMoveTime = millis();

  pinMode(JOYSTICK_SW, INPUT_PULLUP); 
}


void loop() {
  if (!gameOverFlag) {
    unsigned long currentTime = millis();
    if (currentTime - lastMoveTime >= speed) {
      moveSnake();
      lastMoveTime = currentTime;
    }

    checkCollision();
    

    // Gestion du joystick
    int xValue = analogRead(JOYSTICK_X);
    int yValue = analogRead(JOYSTICK_Y);

    if (xValue < 100) { 
      changeDirection(3); 
    } else if (xValue > 900) { 
      changeDirection(1); 
    } else if (yValue < 100) { 
      changeDirection(0); 
    } else if (yValue > 900) { 
      changeDirection(2); 
    }


    if (restartGame) {
      resetGame();
    }
  } else {
    
    displayScore();
    
    
    if (digitalRead(JOYSTICK_SW) == HIGH) {
      restartGame = true; 
    }
  }

  FastLED.show();
}

void moveSnake() {
  int newHeadX = snakeX[0];
  int newHeadY = snakeY[0];

  switch (direction) {
    case 0:
      newHeadY--;
      break;
    case 1: 
      newHeadX++;
      break;
    case 2: 
      newHeadY++;
      break;
    case 3: 
      newHeadX--;
      break;
  }

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] = newHeadX;
  snakeY[0] = newHeadY;

  if (snakeX[0] == appleX && snakeY[0] == appleY) {
    snakeLength++;
    spawnApple();

    if (speed > 20) {
      speed -= 5;
    }
  }

  clearBoard();
  drawSnake();
  drawApple();
}

void checkCollision() {
  if (snakeX[0] < 0 || snakeX[0] >= 8 || snakeY[0] < 0 || snakeY[0] >= 8) {
    gameOver();
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
      gameOver();
    }
  }
}

void spawnApple() {
  appleX = random(8);
  appleY = random(8);
}

void drawSnake() {
  for (int i = 0; i < snakeLength; i++) {
    leds[snakeX[i] + snakeY[i] * 8] = CRGB(0, 255, 0); 
  }
}

void drawApple() {
  leds[appleX + appleY * 8] = CRGB(255, 0, 0); 
}

void clearBoard() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0); 
  }
}

void displayScore() {
  clearBoard();

  int tens = applesEaten / 10;
  int units = applesEaten % 10; 

  int scoreIndex = 6; 

  if (tens > 0) {
    leds[scoreIndex] = CRGB(255, 0, 0);
    scoreIndex--;
    leds[scoreIndex] = CRGB(255, 0, 0);
    scoreIndex--;
  }

  leds[scoreIndex] = CRGB(255, 0, 0);
  scoreIndex--;

  FastLED.show();
}


void gameOver() {
  gameOverFlag = true;
  clearBoard();
  displayScore();
  FastLED.show();
  restartGame = false; 
}

void resetGame() {
  snakeLength = 3;
  direction = 1;
  speed = 150;
  gameOverFlag = false;
  applesEaten = 0;

  snakeX[0] = 3;
  snakeY[0] = 3;

  spawnApple();

  clearBoard();

  restartGame = false;

}

void changeDirection(int newDirection) {
  if ((newDirection == 0 && direction != 2) ||  
      (newDirection == 1 && direction != 3) ||  
      (newDirection == 2 && direction != 0) ||   
      (newDirection == 3 && direction != 1)) { 
    direction = newDirection;
  }
}