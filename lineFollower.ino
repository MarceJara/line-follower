#include <QTRSensors.h>
#include "PINConfig.h"
#include "Functions.h"

// función para pulsar el botón y esperar que deje de pulsarlo
#define esperarBoton() while(!digitalRead(BUTTONPIN)); while(digitalRead(BUTTONPIN))

// estructura para los sensores QTRSensorsAnalog
QTRSensors qtra;

// arreglo para almacenamiento de valores por sensor
unsigned int sensorValues[NUM_SENSORS];



void setup()
{
  // configurar sensor qtr
  qtra.setTypeAnalog();
  qtra.setSensorPins((const uint8_t[]){A7, A6, A5, A4, A3, A2, A1, A0}, NUM_SENSORS);
  qtra.setEmitterPin(EMITTER_PIN);

  // inicializar pines de salida
  pinMode(LEDPIN          ,OUTPUT);
  pinMode(STANDBY         ,OUTPUT);
  pinMode(MOTORRIGHT_DIR_A ,OUTPUT);
  pinMode(MOTORRIGHT_DIR_B ,OUTPUT);
  pinMode(MOTORRIGHT_PWM   ,OUTPUT);
  pinMode(MOTORLEFT_DIR_A ,OUTPUT);
  pinMode(MOTORLEFT_DIR_B ,OUTPUT);
  pinMode(MOTORLEFT_PWM   ,OUTPUT);
  pinMode(BUTTONPIN       ,INPUT);

  // presiona botón para activar calibración
  while ( digitalRead(BUTTONPIN) );

  // calibrar sensores QTRA, titilando LED como guía
  for ( int i=0; i<70; i++)
  {
    digitalWrite(LEDPIN, HIGH); delay(20);
    qtra.calibrate();
    digitalWrite(LEDPIN, LOW);  delay(20);
  }

  // apagar LED
  digitalWrite(LEDPIN, LOW);

  // presionar botón para correr el robot
  while ( digitalRead(BUTTONPIN) );

  // esperar 5 segundos 
  delay(500);
  
  // mover el robot suave para ganar inercia
  setMotors(90, 90);
  
  // durante 0.3 segundos
  delay(300);
}

unsigned int position = 0; // posición actual de los sensores
int derivative = 0;        // derivada
int proportional = 0;      // proporcional
int power_difference = 0;  // velocidad diferencial

// Máxima velocidad en el poder diferencial
int max = 255; // 127

// Ultima Proporcional
int last_proportional;

// Constantes Proporcional y Derivativa
float KP = 0.17; // 0.17 - 0.9
float KD = 2.2; // 2.2 - 2.5

// Constante para Rango de Freno (Range Brake)
#define RANGEBRAKE 3500 // 3500

void loop()
{   
  // Obtiene la posición de la linea
  // Aquí no estamos interesados ​​en los valores 
  // individuales de cada sensor
  //position = qtra.readLine(sensorValues);
  position = qtra.readLineBlack(sensorValues);

  // El término proporcional debe ser 0 cuando estamos en línea
  proportional = ((int)position) - 3500;

  // Si entra en el rango de freno, aplicarlo en la 
  // direccion de la curva
  if ( proportional <= -RANGEBRAKE )
  {
    setMotorRight(0);
    setBrake(true,false,255);
    delay(1);
  }
  else if ( proportional >= RANGEBRAKE )
  {
    setMotorLeft(0);
    setBrake(false,true,255);
    delay(1);
  }

  // Calcula el término derivativo (cambio) de la posición
  derivative = proportional - last_proportional;

  // Recordando la última posición
  last_proportional = proportional;

  // Calcula la diferencia entre la potencia de los dos motores [ m1 - m2 ]. 
  // Si es un número positivo, el robot gira a la [ derecha ] 
  // Si es un número negativo, el robot gira a la [ izquierda ]
  //  y la magnitud del número determina el ángulo de giro.
  int power_difference = ( proportional * KP ) + ( derivative * KD );

  // Si velocidad diferencial es mayor a la posible tanto positiva como negativa,
  // asignar la máxima permitida
  if ( power_difference > max ) power_difference = max; 
  else if ( power_difference < -max ) power_difference = -max;

  // Asignar velocidad calculada en el poder diferencial de los motores
  ( power_difference < 0 ) ? 
    setMotors(max+power_difference, max) : setMotors(max, max-power_difference);
}