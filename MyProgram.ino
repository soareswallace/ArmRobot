// Wallace Soares - wssj2@cin.ufpe.br - 11/2019
// Josenildo Araújo - jva@cin.ufpe.br - 11/2019

#include <ax12.h>
#include <BioloidController.h>
int posMotor3 = 0;
int posMotor4 = 0;
int posMotor5 = 0;
int isCrashing = 0;

// Levar de grau para posição
int degree2pos(int degree)
{
  if (degree<-150)
    degree = -150;
  if (degree>150)
    degree = 150;
    
  int pos = (int)(1023*(float(degree+150)/(300.0)));
  
  return pos;
}

void printPosition(){
  
  int pos2 = ax12GetRegister(2, 36, 2);
  int pos3 = ax12GetRegister(3, 36, 2);
  int pos4 = ax12GetRegister(4, 36, 2);
  int pos5 = ax12GetRegister(5, 36, 2);
  int pos6 = ax12GetRegister(6, 36, 2);
  delay(500);
  Serial.print("Motor 2: ");
  Serial.println(pos2degree(pos2));
  Serial.print("Motor 3: ");
  Serial.println(pos2degree(pos3));
  Serial.print("Motor 4: ");
  Serial.println(pos2degree(pos4));
  Serial.print("Motor 5: ");
  Serial.println(pos2degree(pos5));
  Serial.print("Garra: ");
  Serial.println(pos2degree(pos6));
}

//Levar de posição para grau
int pos2degree(int pos){

  float deg = (300*(float(pos)/1023.0));

  int degree = (int) (deg - 150);  

  return degree;
}

int getArmyAngle(int motor) {
  int pos = ax12GetRegister(motor, 36, 2);
  return pos2degree(pos);
}

//Função para imprimir posição atual dos motores do braço

int checkLimitsAndReturnCorrectValue(int motor, int pos) {    
    if (motor == 3) {
        if (pos < 0) {
            posMotor3 = 0;
            return 0;
        }
        if (pos > 85) {
            posMotor3 = 85;
            return posMotor3;
        }
        return posMotor3;
    }
    
    else if (motor == 4) {
        if (posMotor3 + posMotor4 > 95) {
            posMotor4 = 95-posMotor3;
        }
        return posMotor4;
    }    

    else if (motor == 5) {
        if (posMotor3 + posMotor4 + posMotor5 > 160) {
            posMotor5 = 160-(posMotor3 + posMotor4);
        } 
        return posMotor5;
    }
}

void setUniquePosition(int motor, int pos) {
    int correctValue = checkLimitsAndReturnCorrectValue(motor, pos);
    SetPosition(motor, degree2pos(correctValue));
    delay(500);
}

void setMultiplePosition(int mot3, int mot4, int mot5) {
    posMotor3 = mot3;
    posMotor4 = mot4;
    posMotor5 = mot5;
    setUniquePosition(3,mot3);
    setUniquePosition(4,mot4);
    setUniquePosition(5,mot5);
    delay(2000);
}

void setRotorBasal(int mot2) {
    SetPosition(2,degree2pos(mot2));
    delay(2000);
}

void setTorqueLimitForAxes(int torque, int torqueMotor3) {
    //Mudar velocidade
  ax12SetRegister2(2,AX_TORQUE_LIMIT_L,torque);
  ax12SetRegister2(3,AX_TORQUE_LIMIT_L,torqueMotor3);
  ax12SetRegister2(4,AX_TORQUE_LIMIT_L,torque);
  ax12SetRegister2(5,AX_TORQUE_LIMIT_L,torque);
  ax12SetRegister2(6,AX_TORQUE_LIMIT_L,torque);
  delay(2000);
}

void setVelocityForAxes(int mot2, int mot3, int mot4, int mot5, int mot6) {
  ax12SetRegister2(2,32,mot2);
  ax12SetRegister2(3,32,mot3);
  ax12SetRegister2(4,32,mot4);
  ax12SetRegister2(5,32,mot5);
  ax12SetRegister2(6,32,mot6);
  delay(2000);
}

/*
  Description -> For lock variable: 0 for lock,1 for unlock
  int motorNumber
  int lock
*/
void lockMotor(int motorNumber, int lock) {
    ax12SetRegister(motorNumber,AX_TORQUE_ENABLE,lock);
    delay(2000);
}

void activateLed(int motorNumber, int active) {
    ax12SetRegister(motorNumber,AX_LED,active);
    delay(2000);
}

void setClawPosition(int pos) {
    SetPosition(6,degree2pos(pos));
    delay(2000);
}


//Função para colocar braço na posição inicial
void goHome(){
  
  Serial.println('Entrei no goHome');
  setTorqueLimitForAxes(400, 600);
  setVelocityForAxes(75, 75, 75, 75, 75);
  setMultiplePosition(0,0,0);
  setMultiplePosition(73,82,67);
  Serial.println(1);
  setMultiplePosition(82,57,72);
  Serial.println(2);
  setMultiplePosition(25,49,82);
  Serial.println(3);
  setMultiplePosition(87,14,85);
  Serial.println(4);
  setMultiplePosition(86,44,54);
  Serial.println(5);
  setMultiplePosition(13,38,75);
  Serial.println(6);
  setMultiplePosition(71,86,28);
  Serial.println(7);
  setMultiplePosition(3,76,78);
  Serial.println(8);
  setMultiplePosition(61,68,44);
  Serial.println(9);
  setMultiplePosition(35,59,59);
  Serial.println(10);
  setMultiplePosition(64,3,40);
  Serial.println(11);
  setMultiplePosition(4,9,58);
  Serial.println(12);
  setMultiplePosition(63,29,81);
  Serial.println(13);
  setMultiplePosition(3,39,21);
  Serial.println(14);
  setMultiplePosition(69,72,56);
  Serial.println(15);
  setMultiplePosition(44,40,26);
  Serial.println(16);
  setMultiplePosition(64,68,40);
  Serial.println(17);
  setMultiplePosition(61,59,61);
  Serial.println(18);
  setMultiplePosition(11,45,83);
  Serial.println(19);
  setMultiplePosition(31,53,50);
  Serial.println(20);
  setMultiplePosition(68,23,1);
  Serial.println(21);
  setMultiplePosition(63,80,83);
  Serial.println(22);
  setMultiplePosition(49,12,63);
  Serial.println(23);
  setMultiplePosition(23,76,44);
  Serial.println(24);
  setMultiplePosition(73,22,77);
  Serial.println(25);
  setMultiplePosition(31,18,45);
  Serial.println(26);
  setMultiplePosition(55,43,27);
  Serial.println(27);
  setMultiplePosition(75,53,9);
  Serial.println(28);
  setMultiplePosition(83,26,46);
  Serial.println(29);
  setMultiplePosition(68,34,12);
  Serial.println(30);
  setMultiplePosition(7,5,6);
  Serial.println(31);
  setMultiplePosition(70,84,67);
  Serial.println(32);
  setMultiplePosition(51,42,88);
  Serial.println(33);
  setMultiplePosition(30,15,53);
  Serial.println(34);
  setMultiplePosition(28,48,60);
  Serial.println(35);
  setMultiplePosition(54,24,28);
  Serial.println(36);
  setMultiplePosition(62,67,9);
  Serial.println(37);
  setMultiplePosition(8,21,74);
  Serial.println(38);
  setMultiplePosition(14,74,7);
  Serial.println(39);
  setMultiplePosition(90,7,10);
  Serial.println(40);
  setMultiplePosition(10,87,89);
  Serial.println(41);
  setMultiplePosition(70,74,66);
  Serial.println(42);
  setMultiplePosition(8,36,43);
  Serial.println(43);
  setMultiplePosition(72,39,74);
  Serial.println(44);
  setMultiplePosition(16,24,64);
  Serial.println(45);
  setMultiplePosition(12,78,14);
  Serial.println(46);
  setMultiplePosition(49,13,64);
  Serial.println(47);
  setMultiplePosition(56,32,2);
  Serial.println(48);
  setMultiplePosition(36,7,47);
  Serial.println(49);
  setMultiplePosition(11,17,47);
  Serial.println(50);
  setMultiplePosition(73,82,11);
Serial.println(1);
setMultiplePosition(82,57,9);
Serial.println(2);
setMultiplePosition(25,49,86);
Serial.println(3);
setMultiplePosition(87,14,87);
Serial.println(4);
setMultiplePosition(86,44,72);
Serial.println(5);
setMultiplePosition(13,38,82);
Serial.println(6);
setMultiplePosition(71,86,59);
Serial.println(7);
setMultiplePosition(3,76,84);
Serial.println(8);
setMultiplePosition(61,68,67);
Serial.println(9);
setMultiplePosition(35,59,15);
Serial.println(10);
setMultiplePosition(64,3,25);
Serial.println(11);
setMultiplePosition(4,9,74);
Serial.println(12);
setMultiplePosition(63,29,86);
Serial.println(13);
setMultiplePosition(3,39,34);
Serial.println(14);
setMultiplePosition(69,72,17);
Serial.println(15);
setMultiplePosition(44,40,58);
Serial.println(16);
setMultiplePosition(64,68,25);
Serial.println(17);
setMultiplePosition(61,59,15);
Serial.println(18);
setMultiplePosition(11,45,86);
Serial.println(19);
setMultiplePosition(31,53,20);
Serial.println(20);
setMultiplePosition(68,23,46);
Serial.println(21);
setMultiplePosition(63,80,86);
Serial.println(22);
setMultiplePosition(49,12,13);
Serial.println(23);
setMultiplePosition(23,76,23);
Serial.println(24);
setMultiplePosition(73,22,84);
Serial.println(25);
setMultiplePosition(31,18,23);
Serial.println(26);
setMultiplePosition(55,43,32);
Serial.println(27);
setMultiplePosition(75,53,49);
Serial.println(28);
setMultiplePosition(83,26,68);
Serial.println(29);
setMultiplePosition(68,34,51);
Serial.println(30);
setMultiplePosition(7,5,48);
Serial.println(31);
setMultiplePosition(70,84,12);
Serial.println(32);
setMultiplePosition(51,42,1);
Serial.println(33);
setMultiplePosition(30,15,71);
Serial.println(34);
setMultiplePosition(28,48,15);
Serial.println(35);
setMultiplePosition(54,24,59);
Serial.println(36);
setMultiplePosition(62,67,41);
Serial.println(37);
setMultiplePosition(8,21,82);
Serial.println(38);
setMultiplePosition(14,74,48);
Serial.println(39);
setMultiplePosition(90,7,40);
Serial.println(40);
setMultiplePosition(10,87,0);
Serial.println(41);
setMultiplePosition(70,74,78);
Serial.println(42);
setMultiplePosition(8,36,23);
Serial.println(43);
setMultiplePosition(72,39,82);
Serial.println(44);
setMultiplePosition(16,24,13);
Serial.println(45);
setMultiplePosition(12,78,52);
Serial.println(46);
setMultiplePosition(49,13,77);
Serial.println(47);
setMultiplePosition(56,32,46);
Serial.println(48);
setMultiplePosition(36,7,22);
Serial.println(49);
setMultiplePosition(11,17,22);
Serial.println(50);
  setClawPosition(0);  
  setRotorBasal(0);
  printPosition();
  Serial.println('sai do goHome');
  delay(8000);
  
  //'Desligar' o robo
  lockMotor(1, 0);
  lockMotor(2, 0);
  lockMotor(3, 0);
  lockMotor(4, 0);
  lockMotor(5, 0);
  lockMotor(6, 0);  
}

void setup() {
  Serial.begin(9600);
  goHome();
}

void loop() {
  // put your main code here, to run repeatedly:
}
