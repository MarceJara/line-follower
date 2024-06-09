#include "PINConfig.h"

// función Velocidad Motor Izquierdo
void setMotorLeft(int value)
{
  if ( value >= 0 )
  {
    // si valor positivo vamos hacia adelante
    digitalWrite(MOTORRIGHT_DIR_A,HIGH);
    digitalWrite(MOTORRIGHT_DIR_B,LOW);
  }
  else
  {
    // si valor negativo vamos hacia atras
    digitalWrite(MOTORRIGHT_DIR_A,LOW);
    digitalWrite(MOTORRIGHT_DIR_B,HIGH);
    value *= -1;
  }

  // Setea Velocidad
  analogWrite(MOTORRIGHT_PWM,value);
}

// función Velocidad Motor Derecho
void setMotorRight(int value)
{  
  if ( value >= 0 )
  {
    // si valor positivo vamos hacia adelante
    digitalWrite(MOTORLEFT_DIR_A,HIGH);
    digitalWrite(MOTORLEFT_DIR_B,LOW);
  }
  else
  {
    // si valor negativo vamos hacia atras
    digitalWrite(MOTORLEFT_DIR_A,LOW);
    digitalWrite(MOTORLEFT_DIR_B,HIGH);
    value *= -1;
  }    

  // Setea Velocidad
  analogWrite(MOTORLEFT_PWM,value);
}

// función Velocidad Motores
void setMotors(int left, int right)
{
  digitalWrite(STANDBY,HIGH);
  setMotorLeft(left);
  setMotorRight(right);
}

// función Freno en Motores
void setBrake(boolean left, boolean right, int value)
{
  // pin STAND BY
  digitalWrite(STANDBY,HIGH);

  if ( left )
  {
    // pines LEFT motor
    digitalWrite(MOTORRIGHT_DIR_A,HIGH);
    digitalWrite(MOTORRIGHT_DIR_B,HIGH);
    analogWrite (MOTORRIGHT_PWM, value);
  }

  if ( right )
  {
    // pines RIGHT motor
    digitalWrite(MOTORLEFT_DIR_A,HIGH);
    digitalWrite(MOTORLEFT_DIR_B,HIGH);
    analogWrite (MOTORLEFT_PWM, value);
  }
}