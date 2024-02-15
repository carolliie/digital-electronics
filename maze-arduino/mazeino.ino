#include <Servo.h>

Servo myServoX;
Servo myServoY;

int ServoXPin = 6;
int ServoYPin = 5;
int ServoXHomePos = 103; 
int ServoYHomePos = 134; 
int ServoXPos = 103; 
int ServoYPos = 135; 
int XAxlePin = A6;      // pinos do joystick A6 e A7
int YAxlePin = A7; 
int XAxleValue = 0;
int YAxleValue = 0;
int Direction = 0;
int range = 12; 
int center = range/2; 
int threshold = range/4; 


void setup() {                  // inicializa os servo-motores e estabelece posições iniciais
myServoX.attach(ServoXPin);
myServoY.attach(ServoYPin);
ServoXPos = ServoXHomePos;
ServoYPos = ServoYHomePos;
myServoX.write(ServoXPos);
myServoY.write(ServoYPos);
Serial.begin(9600);
}

void loop() {                   // lê os valores dos potenciometros (x e y)
XAxleValue = readAxis(XAxlePin);
YAxleValue = readAxis(YAxlePin);

Serial.print(XAxleValue,DEC);
Serial.print(" - ");
Serial.println(YAxleValue,DEC);

// dependendo do valor dos potenciometros, ajusta-se a posição dos servos motores

if (XAxleValue>0) { ServoXPos++; myServoX.write(ServoXPos); delay(1*(7-XAxleValue)); } // se é - vai p uma direção
if (XAxleValue<0) { ServoXPos--; myServoX.write(ServoXPos); delay(1*(7+XAxleValue)); } // se é + vai p outra direção
if (YAxleValue>0) { ServoYPos++; myServoY.write(ServoYPos); delay(1*(7-YAxleValue)); } // ajustei o delay pra 1 p testar se o movimento seria mais rápido
if (YAxleValue<0) { ServoYPos--; myServoY.write(ServoYPos); delay(1*(7+YAxleValue)); }

if (ServoXPos>ServoXHomePos+50) { ServoXPos=ServoXHomePos+50; }
if (ServoXPos<ServoXHomePos-50) { ServoXPos= ServoXHomePos-50; }
if (ServoYPos>ServoYHomePos+50) { ServoYPos=ServoYHomePos+50; }
if (ServoYPos<ServoYHomePos-50) { ServoYPos= ServoYHomePos-50; }
delay(10);
}

int readAxis(int thisAxis) {            

int reading = analogRead(thisAxis);

reading = map(reading, 0, 1023, 0, range);  // lê e mapeia um range específico

int distance = reading - center;            // calcula a distância desde o centro e aplica uma condição p evitar complicações
if (abs(distance) < threshold) {
    distance = 0;
}

return distance;                            // retorna a distancia calculada
}
