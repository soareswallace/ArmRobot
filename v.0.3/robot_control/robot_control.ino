#include <ax12.h>
#include <BioloidController.h>

#define base 2
#define ombro 3
#define cotovelo 4
#define pulso 5
#define garra 6

int blinkStatus = 0;

void setup()
{ 
  Serial.begin(9600);
    changeVelocity();
    delay(100);
  //Serial.read();
  Serial.println("start");
}

void loop()
{
  if(blinkStatus >= 1000)
  {
    blinkStatus = 0;
    if(ax12GetRegister(ombro, 25, 1))
    {
      ax12SetRegister(ombro, 25, 0);
      ax12SetRegister(cotovelo, 25, 0);
      ax12SetRegister(pulso, 25, 0);
    }
    else
    {
      ax12SetRegister(ombro, 25, 1);
      ax12SetRegister(cotovelo, 25, 1);
      ax12SetRegister(pulso, 25, 1);
    }
  }
  else blinkStatus = blinkStatus + 1;

  receiveComandsSerial();
  delay(1);
}

void receiveComandsSerial()
{  
    int motor2 = 0;
    int motor3 = 0;
    int motor4 = 0;
    int motor5 = 0;
    
  if(Serial.available()>=12)
  {
    blinkStatus = 1000;
    ax12SetRegister(ombro, 25, 1);
    ax12SetRegister(cotovelo, 25, 1);
    ax12SetRegister(pulso, 25, 1);
    
    Serial.print(Serial.available());
    motor2 = serialReceiveNumber();
    motor3 = serialReceiveNumber();
    motor4 = serialReceiveNumber();
    motor5 = serialReceiveNumber();

    SetPositionMulti(motor2, motor3, motor4, motor5);
    
    //changeVelocity();
    //SetPosition(base,degree2pos(motor2));
    //SetPosition(ombro,degree2pos(motor3));
    //SetPosition(cotovelo,degree2pos(motor4));
    //SetPosition(pulso,degree2pos(motor5));
    //SetPosition(6,512);
    
    Serial.print(" | ");
    Serial.print(GetPositionExt(2));
    Serial.print(", ");
    Serial.print(GetPositionExt(3));
    Serial.print(", ");
    Serial.print(GetPositionExt(4));
    Serial.print(", ");
    Serial.println(GetPositionExt(5));
  }
  else if(Serial.available())
  {
    blinkStatus = 1000;
    ax12SetRegister(ombro, 25, 0);
    ax12SetRegister(cotovelo, 25, 0);
    ax12SetRegister(pulso, 25, 0);
    
    if(Serial.peek() == 'c'){
      catchObject(); // catch
      Serial.read(); 
    }
    if(Serial.peek() == 'd'){
      dropObject(); // drop
      Serial.read(); 
    }
    if(Serial.peek() == 'h'){
      homePos(); // home
      Serial.read(); 
    }
    if(Serial.peek() == 's') {
      sampleMove(); // sample
      Serial.read(); 
    }
    if(Serial.peek() == 'k'){
      turnOff(); // kill
      Serial.read(); 
    }
    if(Serial.peek() == 'z'){
      zeroPos(); // zero
      Serial.read(); 
    }
    if(Serial.peek() == 'p'){
       printPosition(); // printar posições
       Serial.read(); 
    }
    if(Serial.peek() == 'i'){
       catchColorPos(); // setar posições para pegar imagem da co
       Serial.read(); 
    }
    
    //Serial.read();
  }
}

int serialReceiveNumber()
{
  int signal = Serial.read();
  int pos1 = Serial.read();
  int pos2 = Serial.read();
  int res = 0;
  
  if(signal) res = ((pos2 * 256) + pos1) * (-1);
  else res = ((pos2 * 256) + pos1);
  return res;
}

void sampleMove()
{
  SetPositionMulti(10,40,-45,60);
  SetPositionMulti(-7,26,13,-5);
  SetPositionMulti(-60,50,-30,10);
}

void zeroPos()
{
  SetPositionMulti(0, 0, 0, 0);
  SetPositionExt(garra, 0);
}

void catchColorPos()
{
  SetPositionMulti(0, 65, 64, 15);
}

void luz(int num)
{
  ax12SetRegister(base, 25, num);
  ax12SetRegister(ombro, 25, num);
  ax12SetRegister(cotovelo, 25, num);
  ax12SetRegister(pulso, 25, num);
}

void catchObject()
{
  SetPositionExt(garra, -110);
}

void dropObject()
{
  SetPositionExt(garra, 0);
  Relax(garra);
}

void changeVelocity()
{
  //Mudar velocidade

  //torque
  ax12SetRegister2(base, 14, 150);
  ax12SetRegister2(ombro, 14, 800);
  ax12SetRegister2(cotovelo, 14, 700);
  ax12SetRegister2(pulso, 14, 400);
  ax12SetRegister2(garra, 14, 500);
  
  //velocidade
  ax12SetRegister2(base, 32, 100);
  ax12SetRegister2(ombro, 32, 100);
  ax12SetRegister2(cotovelo, 32, 100);
  ax12SetRegister2(pulso, 32, 100);
  ax12SetRegister2(garra, 32, 500);
}

void turnOff()
{
 //'Desligar' o robo
  Relax(base);
  Relax(ombro);
  Relax(cotovelo);
  Relax(pulso);
  Relax(garra);
  
  ax12SetRegister(base,AX_LED,0);
  ax12SetRegister(ombro,AX_LED,0);
  ax12SetRegister(cotovelo,AX_LED,0);
  ax12SetRegister(pulso,AX_LED,0);
  ax12SetRegister(garra,AX_LED,0);
}

//Função para colocar braço na posição inicial
void homePos()
{ 
  //Mudar posição para inicial (ou home)
  SetPositionMulti(0, 142, -140, -55);
  
  //'Desligar' o robo
  turnOff();
}

//Levar de posição para grau
float pos2degree(int pos)
{
  float degree = (300*(float(pos)/1023.0)) -150;
  return degree;
}

// Levar de grau para posição
int degree2pos(float degree)
{
  if(degree < -150) degree = -150;
  if(degree > 150) degree = 150;
    
  int pos = (int)(1023*(float(degree+150)/(300.0)));
  return pos;
}

//Função para imprimir posição atual dos motores do braço
void printPosition()
{
  Serial.print("Base: ");
  Serial.println(GetPositionExt(base));
  Serial.print("Ombro: ");
  Serial.println(GetPositionExt(ombro));
  Serial.print("Cotovelo: ");
  Serial.println(GetPositionExt(cotovelo));
  Serial.print("Pulso: ");
  Serial.println(GetPositionExt(pulso));
  Serial.print("Garra: ");
  Serial.println(GetPositionExt(garra));
}

float GetPositionExt(int num_motor)
{
  float degree = pos2degree(GetPosition(num_motor));
  return degree;
}

int isMoving(int num_motor)
{
   return ax12GetRegister(num_motor, 46, 1);
}

//Nova função setPosition
void SetPositionExt(int num_motor, float num_pos)
{
  SetPosition(num_motor, degree2pos(num_pos));
  float curPos = GetPositionExt(num_motor);
  while(isMoving(num_motor))
  { 
    delay(150);
    if(GetPositionExt(num_motor) > curPos -1 && GetPositionExt(num_motor) < curPos +1)
    {
       SetPosition(num_motor, GetPosition(num_motor));
       return;
     }
     curPos = GetPositionExt(num_motor);
  }
}

// move todos os braços de uma vez, espera todos chegarem à posição esperada antes de continuar
void SetPositionMulti(float num_pos1, float num_pos2, float num_pos3, float num_pos4)
{
  SetPosition(base, degree2pos(num_pos1));
  SetPosition(ombro, degree2pos(num_pos2));
  SetPosition(cotovelo, degree2pos(num_pos3));
  SetPosition(pulso, degree2pos(num_pos4));
  int i=0;
  
  while((isMoving(base) > 0 || isMoving(ombro) > 0 || isMoving(cotovelo) > 0 || isMoving(pulso) > 0) && i<50)
  {  
    if(!isMoving(base)) Relax(base);  // Relaxar base imediatamente assim que possível, a fim de evitar "tremedeiras"
    delay(100);
    i++;
  }
  if(i>=50)
  {
    SetPositionMulti(num_pos1, num_pos2, num_pos3, num_pos4);
    return;
  }
  Relax(base);
}
