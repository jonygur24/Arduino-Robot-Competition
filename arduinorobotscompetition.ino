const float Kp = 0.042;
const float Kd = 0.0075;
const float Ki = 0.00039062;
const int sensor[5] = {4, 5, 6, 7, 8};
const int num_L = 130;
const int num_R = num_L+19;
const int min = 5;
const int mid = 22;
const int max = 50;
const int in1 = A0;
const int in2 = A1;
const int ena = 11;
const int in3 = A2;
const int in4 = A3;
const int enb = 10;
const int MAX_STOP = 20;
float correction = 0.0;
int sensorValues[5];
int value = 0;
int position = 0;
int Error = 0;
int Last_Error = 0;
int Sum_Error = 0;
int count = 0;




void setup() 
{
for (int i = 0; i < 5; i++) 
{
   pinMode(sensor[i], INPUT);
}
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  Serial.begin(9600);
}

void calculate_Sensor()
{
  for (int i = 0; i < 5; i++) 
  {
  sensorValues[i] = (!digitalRead(sensor[i])); 
  }

  int sensorSum = sensorValues[0] + sensorValues[1] + sensorValues[2] + sensorValues[3] + sensorValues[4];

  if (sensorValues[0] == 1 || sensorValues[1] == 1 || sensorValues[2] == 1 || sensorValues[3] == 1 || sensorValues[4] == 1) 
  {
    position = ((1000 * sensorValues[1] + 2000 * sensorValues[2] + 3000 * sensorValues[3] + 4000 * sensorValues[4]) /
                sensorSum);
    value = 1;  
    Error = position - 2000;       
  }

/*
    if(sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 1 && sensorValues[3] == 1 && sensorValues[4] == 1)
  {
       position = ((1000 * sensorValues[1] + 2000 * sensorValues[2] + 3000 * sensorValues[3] + 4000 * sensorValues[4]) /
                sensorSum);
    value = 2;  
    Error = position - 2000;   
  }
  */

  if (sensorValues[0] == 0 && sensorValues[1] == 0 && sensorValues[2] == 0 && sensorValues[3] == 0 && sensorValues[4] == 0) 
  {
    value = 0;
  }
}

void calculate_Error() 
{
  Sum_Error += Error;
  correction = Kp * Error + Kd * (Error - Last_Error) + Ki * Sum_Error;
  Last_Error = Error;
}

void steering()
{
  int updated_speed_Right = num_R+correction;
  int updated_speed_Left = num_L-correction;

  if (updated_speed_Right<0)
  {
    analogWrite(ena,0);
    digitalWrite(in2,LOW);
    digitalWrite(in1,HIGH);
    analogWrite(enb,updated_speed_Left);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    delay(min);
  }
  if (updated_speed_Left<0)
  {
    analogWrite(ena,updated_speed_Right);
    digitalWrite(in2,LOW);
    digitalWrite(in1,HIGH);
    analogWrite(enb,0);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    delay(min);
  }
 if(updated_speed_Right*updated_speed_Left>0)
 {
  analogWrite(ena,updated_speed_Right);
  digitalWrite(in2,LOW);
  digitalWrite(in1,HIGH);
  analogWrite(enb,updated_speed_Left);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  delay(min);
 }
}

void steeringSlow()
{
  int updated_speed_Right = (num_R+correction)*0.38;
  int updated_speed_Left = (num_L-correction)*0.38;

  if (updated_speed_Right<0)
  {
    analogWrite(ena,0);
    digitalWrite(in2,LOW);
    digitalWrite(in1,HIGH);
    analogWrite(enb,updated_speed_Left);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    delay(mid);
  }

  if (updated_speed_Left<0)
  {
    analogWrite(ena,updated_speed_Right);
    digitalWrite(in2,LOW);
    digitalWrite(in1,HIGH);
    analogWrite(enb,0);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    delay(mid);
  }
 if(updated_speed_Right*updated_speed_Left>0)
 {
  analogWrite(ena,updated_speed_Right);
  digitalWrite(in2,LOW);
  digitalWrite(in1,HIGH);
  analogWrite(enb,updated_speed_Left);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  delay(mid);
 }
}

void revers()
{
  analogWrite(ena, num_R);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  analogWrite(enb, num_L);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(mid);
}


void loop()
{
calculate_Sensor();

if(value == 1) 
{
  calculate_Error();
  steering();
}

while (value == 0)
{
    if(Error < 1000 || Error > -1000)
  {
  revers();
  steering();
  }
  if(Error == 1)
  {
    steering();
  }
  else
  {
   steeringSlow();
  }
  calculate_Sensor();
  
}

}
