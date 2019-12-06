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
  setMultiplePosition(38,4,81);
  setRotorBasal(85);
  Serial.println(1);
  setMultiplePosition(44,44,30);
  setRotorBasal(81);
  Serial.println(2);
  setMultiplePosition(33,10,70);
  setRotorBasal(35);
  Serial.println(3);
  setMultiplePosition(22,36,9);
  setRotorBasal(12);
  Serial.println(4);
  setMultiplePosition(85,86,52);
  setRotorBasal(5);
  Serial.println(5);
  setMultiplePosition(21,32,74);
  setRotorBasal(1);
  Serial.println(6);
  setMultiplePosition(4,15,58);
  setRotorBasal(66);
  Serial.println(7);
  setMultiplePosition(58,41,49);
  setRotorBasal(27);
  Serial.println(8);
  setMultiplePosition(67,17,62);
  setRotorBasal(17);
  Serial.println(9);
  setMultiplePosition(33,56,70);
  setRotorBasal(7);
  Serial.println(10);
  setMultiplePosition(84,70,44);
  setRotorBasal(39);
  Serial.println(11);
  setMultiplePosition(40,28,46);
  setRotorBasal(46);
  Serial.println(12);
  setMultiplePosition(74,72,58);
  setRotorBasal(34);
  Serial.println(13);
  setMultiplePosition(73,48,32);
  setRotorBasal(85);
  Serial.println(14);
  setMultiplePosition(79,50,56);
  setRotorBasal(53);
  Serial.println(15);
  setMultiplePosition(19,27,42);
  setRotorBasal(21);
  Serial.println(16);
  setMultiplePosition(76,18,20);
  setRotorBasal(15);
  Serial.println(17);
  setMultiplePosition(20,39,28);
  setRotorBasal(83);
  Serial.println(18);
  setMultiplePosition(39,17,81);
  setRotorBasal(88);
  Serial.println(19);
  setMultiplePosition(39,10,23);
  setRotorBasal(37);
  Serial.println(20);
  setMultiplePosition(54,24,54);
  setRotorBasal(64);
  Serial.println(21);
  setMultiplePosition(20,11,27);
  setRotorBasal(29);
  Serial.println(22);
  setMultiplePosition(38,46,8);
  setRotorBasal(24);
  Serial.println(23);
  setMultiplePosition(72,3,84);
  setRotorBasal(66);
  Serial.println(24);
  setMultiplePosition(44,52,21);
  setRotorBasal(41);
  Serial.println(25);
  setMultiplePosition(87,49,47);
  setRotorBasal(21);
  Serial.println(26);
  setMultiplePosition(44,56,61);
  setRotorBasal(36);
  Serial.println(27);
  setMultiplePosition(33,89,3);
  setRotorBasal(80);
  Serial.println(28);
  setMultiplePosition(82,72,9);
  setRotorBasal(24);
  Serial.println(29);
  setMultiplePosition(30,61,12);
  setRotorBasal(65);
  Serial.println(30);
  setMultiplePosition(10,59,44);
  setRotorBasal(70);
  Serial.println(31);
  setMultiplePosition(64,81,80);
  setRotorBasal(30);
  Serial.println(32);
  setMultiplePosition(63,18,3);
  setRotorBasal(67);
  Serial.println(33);
  setMultiplePosition(45,43,81);
  setRotorBasal(55);
  Serial.println(34);
  setMultiplePosition(56,77,72);
  setRotorBasal(52);
  Serial.println(35);
  setMultiplePosition(16,22,80);
  setRotorBasal(3);
  Serial.println(36);
  setMultiplePosition(44,15,88);
  setRotorBasal(64);
  Serial.println(37);
  setMultiplePosition(45,42,5);
  setRotorBasal(61);
  Serial.println(38);
  setMultiplePosition(4,6,47);
  setRotorBasal(9);
  Serial.println(39);
  setMultiplePosition(74,74,65);
  setRotorBasal(13);
  Serial.println(40);
  setMultiplePosition(59,47,88);
  setRotorBasal(58);
  Serial.println(41);
  setMultiplePosition(72,41,39);
  setRotorBasal(74);
  Serial.println(42);
  setMultiplePosition(8,12,16);
  setRotorBasal(35);
  Serial.println(43);
  setMultiplePosition(75,72,5);
  setRotorBasal(36);
  Serial.println(44);
  setMultiplePosition(47,38,59);
  setRotorBasal(57);
  Serial.println(45);
  setMultiplePosition(26,39,1);
  setRotorBasal(89);
  Serial.println(46);
  setMultiplePosition(15,10,34);
  setRotorBasal(18);
  Serial.println(47);
  setMultiplePosition(44,31,86);
  setRotorBasal(83);
  Serial.println(48);
  setMultiplePosition(5,66,24);
  setRotorBasal(38);
  Serial.println(49);
  setMultiplePosition(49,85,38);
  setRotorBasal(88);
  Serial.println(50);
  setMultiplePosition(27,63,60);
  setRotorBasal(49);
  Serial.println(51);
  setMultiplePosition(63,60,16);
  setRotorBasal(12);
  Serial.println(52);
  setMultiplePosition(90,15,3);
  setRotorBasal(51);
  Serial.println(53);
  setMultiplePosition(79,60,17);
  setRotorBasal(33);
  Serial.println(54);
  setMultiplePosition(41,88,14);
  setRotorBasal(77);
  Serial.println(55);
  setMultiplePosition(58,34,17);
  setRotorBasal(39);
  Serial.println(56);
  setMultiplePosition(43,11,53);
  setRotorBasal(20);
  Serial.println(57);
  setMultiplePosition(35,52,23);
  setRotorBasal(26);
  Serial.println(58);
  setMultiplePosition(56,24,74);
  setRotorBasal(88);
  Serial.println(59);
  setMultiplePosition(66,31,53);
  setRotorBasal(10);
  Serial.println(60);
  setMultiplePosition(82,79,74);
  setRotorBasal(23);
  Serial.println(61);
  setMultiplePosition(53,2,38);
  setRotorBasal(28);
  Serial.println(62);
  setMultiplePosition(15,16,38);
  setRotorBasal(8);
  Serial.println(63);
  setMultiplePosition(54,42,63);
  setRotorBasal(63);
  Serial.println(64);
  setMultiplePosition(57,3,6);
  setRotorBasal(29);
  Serial.println(65);
  setMultiplePosition(48,59,37);
  setRotorBasal(74);
  Serial.println(66);
  setMultiplePosition(65,87,48);
  setRotorBasal(29);
  Serial.println(67);
  setMultiplePosition(10,55,70);
  setRotorBasal(38);
  Serial.println(68);
  setMultiplePosition(8,24,14);
  setRotorBasal(25);
  Serial.println(69);
  setMultiplePosition(40,47,41);
  setRotorBasal(79);
  Serial.println(70);
  setMultiplePosition(47,85,57);
  setRotorBasal(86);
  Serial.println(71);
  setMultiplePosition(22,61,26);
  setRotorBasal(60);
  Serial.println(72);
  setMultiplePosition(63,6,23);
  setRotorBasal(20);
  Serial.println(73);
  setMultiplePosition(60,76,31);
  setRotorBasal(70);
  Serial.println(74);
  setMultiplePosition(61,1,54);
  setRotorBasal(35);
  Serial.println(75);
  setMultiplePosition(82,0,42);
  setRotorBasal(38);
  Serial.println(76);
  setMultiplePosition(41,69,29);
  setRotorBasal(71);
  Serial.println(77);
  setMultiplePosition(42,3,16);
  setRotorBasal(65);
  Serial.println(78);
  setMultiplePosition(43,14,31);
  setRotorBasal(55);
  Serial.println(79);
  setMultiplePosition(17,66,22);
  setRotorBasal(83);
  Serial.println(80);
  setMultiplePosition(24,69,17);
  setRotorBasal(26);
  Serial.println(81);
  setMultiplePosition(8,52,62);
  setRotorBasal(49);
  Serial.println(82);
  setMultiplePosition(38,58,58);
  setRotorBasal(61);
  Serial.println(83);
  setMultiplePosition(57,85,19);
  setRotorBasal(64);
  Serial.println(84);
  setMultiplePosition(21,11,55);
  setRotorBasal(41);
  Serial.println(85);
  setMultiplePosition(41,60,69);
  setRotorBasal(32);
  Serial.println(86);
  setMultiplePosition(60,37,76);
  setRotorBasal(75);
  Serial.println(87);
  setMultiplePosition(23,55,52);
  setRotorBasal(49);
  Serial.println(88);
  setMultiplePosition(78,24,29);
  setRotorBasal(11);
  Serial.println(89);
  setMultiplePosition(85,58,43);
  setRotorBasal(58);
  Serial.println(90);
  setMultiplePosition(49,58,49);
  setRotorBasal(65);
  Serial.println(91);
  setMultiplePosition(47,89,20);
  setRotorBasal(10);
  Serial.println(92);
  setMultiplePosition(10,6,36);
  setRotorBasal(40);
  Serial.println(93);
  setMultiplePosition(33,69,57);
  setRotorBasal(69);
  Serial.println(94);
  setMultiplePosition(84,88,17);
  setRotorBasal(12);
  Serial.println(95);
  setMultiplePosition(63,8,47);
  setRotorBasal(48);
  Serial.println(96);
  setMultiplePosition(78,44,35);
  setRotorBasal(60);
  Serial.println(97);
  setMultiplePosition(67,47,31);
  setRotorBasal(13);
  Serial.println(98);
  setMultiplePosition(53,24,4);
  setRotorBasal(68);
  Serial.println(99);
  setMultiplePosition(22,40,62);
  setRotorBasal(32);
  Serial.println(100);
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
