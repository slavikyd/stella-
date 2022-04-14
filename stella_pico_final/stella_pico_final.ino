// Подключение библиотек для работы с шаговыми двигателями
#include <AccelStepper.h>

// Пины
int angleDirPin = 27;
int angleStepPin = 26;
int rotateDirPin = 21;
int rotateStepPin = 19;

// Коэффициенты  редукторов
int k1 = 1;
int k2 = 1;

// Коэффициенты  драйверов
int q1 = 8;
int q2 = 8;

// Значения углов поворота
double angle = 0;
double rotation = 0;

// Переменные для принятия информации из СОМ-порта
bool newData = false;
String messageFromPC; 
const byte numChars = 64;
char tempChars[numChars];
char receivedChars[numChars];

// Инициализация моторов
AccelStepper angleStep(1, angleStepPin, angleDirPin);
AccelStepper rotateStep(1, rotateStepPin, rotateDirPin);

// Инициализация порта, скоростей, ускорений и пинов концевиков
void setup() {
  Serial.begin(9600);
  pinMode(angleDirPin, OUTPUT);
  angleStep.setMaxSpeed(51200);
  rotateStep.setMaxSpeed(51200);
  angleStep.setAcceleration(51200);
  rotateStep.setAcceleration(51200);
}

//=====================================================================================
// Бесконечный цикл для считывания параметров при их появлении
void loop() {
  digitalWrite(angleDirPin, HIGH);
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    newData = false;
  }
}

//=====================================================================================
// Считывание строки из СОМ-порта
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '$';
  char endMarker = ';';
  char rc;
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // завершаем строку
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}


double double_abs(double d) {
  if (d < 0) {
    return -d;
  }
  return d;
}
//=====================================================================================
// Парсинг полученной строки
void parseData() {      // разделение данных на составляющие части
  char * strtokIndx; // это используется функцией strtok() как индекс
  strtokIndx = strtok(tempChars, " ");     // получаем значение первой переменной - строку
  messageFromPC = strtokIndx; //записываем её в переменную messageFromPC
  if (messageFromPC == "rotation") {
    strtokIndx = strtok(NULL, " "); // продолжаем с последнего индекса
    angle = atof(strtokIndx);    // конвертируем эту составляющую во float
    strtokIndx = strtok(NULL, " ");
    rotation = atof(strtokIndx);     // преобразовываем этот кусок текста во float
    if((rotation > 0) && (rotation < 22.5)){
      // Serial.println("0-22.5");
      if(angle >= 65 || angle < 80){
        rotStepAngle(-20, 8.75 - 35);
      }
      else if(angle >= 80 || angle <= 90){
        rotStepAngle(-10, 8.75 - 35);
      }
      else{
        rotStepAngle(-35, 8.75 - 35);
      }
    }
    else if((22.5 <= rotation) && (rotation < 45)){
        // Serial.println("22.5-45");
        if(angle >= 65 || angle < 80){
          rotStepAngle(-20, 26.25 - 35);
        }
        else if(angle >= 80 || angle <= 90){
          rotStepAngle(-10, 26.25 - 35);
        }
        else{
          rotStepAngle(-35, 26.25 - 35);
        }
    }  
    else if((rotation >= 45) && (rotation < 67.5)){
        // Serial.println("45-67.5");
        if(angle >= 65 || angle < 80){
          rotStepAngle(-20, 43.75 - 35);
        }
        else if(angle >= 80 || angle <= 90){
          rotStepAngle(-10, 43.75 - 35);
        }
        else{
          rotStepAngle(-35, 43.75 - 35);
        }
    }
    else if((rotation >= 67.5) && (rotation < 90)){
            // Serial.println("67.5-90");
            if(angle >= 65 || angle < 80){
              rotStepAngle(-20, 61.25 - 35);
            }
            else if(angle >= 80 || angle <= 90){
              rotStepAngle(-10, 61.25 - 35);
            }
            else{
              rotStepAngle(-35, 61.25 - 35);
            }
    }
    else if((rotation >= 90) && (rotation < 112.5)){
                // Serial.println("90-112.5");
                if(angle >= 65 || angle < 80){
                  rotStepAngle(8.75 - 35, 70 - 50);
                }
                else if(angle >= 80 || angle <= 90){
                  rotStepAngle(8.75 - 35, 70 - 60);
                }
                else{
                  rotStepAngle(8.75 - 35, 70 - 35);
                }
    }
    else if((rotation >= 112.5) && (rotation < 135)){
                  // Serial.println("112.5-135");
                  if(angle >= 65 || angle < 80){
                    rotStepAngle(26.25 - 35, 70 - 50);
                  }
                  else if(angle >= 80 || angle <= 90){
                    rotStepAngle(26.25 - 35, 70 - 60);
                  }
                  else{
                    rotStepAngle(26.25 - 35, 70 - 35);
                  }
    }
    else if((rotation >= 135) && (rotation < 157.5)){
                    // Serial.println("135-157.5");
                  if(angle >= 65 || angle < 80){
                    rotStepAngle(43.75 - 35, 70 - 50);
                  }
                  else if(angle >= 80 || angle <= 90){
                    rotStepAngle(43.75 - 35, 70 - 60);
                  }
                  else{
                    rotStepAngle(43.75 - 35, 70 - 35);
                  }
    }
    else if((rotation >= 157.5) && (rotation < 180)){
                      // Serial.println("157.5-180");
                    if(angle >= 65 || angle < 80){
                      rotStepAngle(61.25 - 35, 70 - 50);
                    }
                    else if(angle >= 80 || angle <= 90){
                      rotStepAngle(61.25 - 35, 70 - 60);
                    }
                    else{
                      rotStepAngle(61.25 - 35, 70 - 35);
                    }
    }
    else if((rotation >= 180) && (rotation < 202.5)){
                        // Serial.println("180-202.5");
                      if(angle >= 65 || angle < 80){
                        rotStepAngle(70 - 50, 61.25 - 35);
                      }
                      else if(angle >= 80 || angle <= 90){
                        rotStepAngle(70 - 60, 61.25 - 35);
                      }
                      else{
                        Serial.println("here");
                        rotStepAngle(70 - 35, 61.25 - 35);
                      }
    }
    else if((rotation >= 202.5) && (rotation < 225)){
                          // Serial.println("202.5-225");
                        if(angle >= 65 || angle < 80){
                          rotStepAngle(70 - 50, 43.75 - 35);
                        }
                        else if(angle >= 80 || angle <= 90){
                          rotStepAngle(70 - 60, 43.75 - 35);
                        }
                        else{
                          rotStepAngle(70 - 35, 43.75  - 35);
                        }
    }
    else if((rotation >= 225) && (rotation < 247.5)){
                            // Serial.println("225-247.5");
                          if(angle >= 65 || angle < 80){
                            rotStepAngle(70 - 50, 26.25 - 35);
                          }
                          else if(angle >= 80 || angle <= 90){
                            rotStepAngle(70 - 60, 26.25 - 35);
                          }
                          else{
                            rotStepAngle(70 - 35, 26.25 - 35);
                          }
    }
    else if((rotation >= 247.5) && (rotation < 270)){
                              // Serial.println("247.5-270");
                            if(angle >= 65 || angle < 80){
                              rotStepAngle(70 - 50, 8.75 - 35);
                            }
                            else if(angle >= 80 || angle <= 90){
                              rotStepAngle(70 - 60, 8.75 - 35);
                            }
                            else{
                              rotStepAngle(70 - 35,8.75  - 35);
                            }
    }
    else if((rotation >= 270) && (rotation < 292.5)){
                                // Serial.println("270-292.5");
                              if(angle >= 65 || angle < 80){
                                rotStepAngle(61.25 - 20, -35);
                              }
                              else if(angle >= 80 || angle <= 90){
                                rotStepAngle(61.25 - 10, -35);
                              }
                              else{
                                rotStepAngle(61.25 - 35, -35);
                              }
    }
    else if((rotation >= 292.5) && (rotation < 315)){
                                  // Serial.println("292.5-315");
                                if(angle >= 65 || angle < 80){
                                  rotStepAngle(43.75 - 20, -35);
                                }
                                else if(angle >= 80 || angle <= 90){
                                  rotStepAngle(43.75 - 10, -35);
                                }
                                else{
                                  rotStepAngle(43.75 - 35, -35);
                                }
    }
    else if((rotation >= 315) && (rotation < 337.5)){
                                    // Serial.println("315-337.5");
                                  if(angle >= 65 || angle < 80){
                                    rotStepAngle(26.25 - 20, -35);
                                  }
                                  else if(angle >= 80 || angle <= 90){
                                    rotStepAngle(26.25 - 10, -35);
                                  }
                                  else{
                                    rotStepAngle(26.25 - 35, -35);
                                  }
    }
    else if((rotation >= 337.5) && (rotation < 360)){
                                      // Serial.println("337.5-360");
                                    if(angle >= 65 || angle < 80){
                                      rotStepAngle(8.75 - 20, -35);
                                    }
                                    else if(angle >= 80 || angle <= 90){
                                      rotStepAngle(8.75 - 10, -35);
                                    }
                                    else{
                                      rotStepAngle(8.75 - 35, -35);
                                    }
    }
  }    
  if (messageFromPC == "homing") {
    Serial.println("homing...");
    homingSteppers();
    Serial.println("homed.");
  }
  String messageFromPC;
}

//=====================================================================================
// Поворот на угол, указанный в СОМ-порте. Index - номер мотора
void rotStepAngle(int angle, int rotation) {
  Serial.print("OK\n");

  angle = ((angle * q1 * k1) / 1.8 * 360 * 4 * 1.25);
  rotation = ((rotation * q1 * k1) / 1.8 * 360 * 4 * 1.25);
  angleStep.moveTo(angle);
  rotateStep.moveTo(rotation);
  while(angleStep.distanceToGo()){angleStep.run();}
  while(rotateStep.distanceToGo()){rotateStep.run();}
  angle = 0;
  rotation = 0;
  
  Serial.print("Final\n");
}

//=====================================================================================
// Функция перемещения к концевикам
void homingSteppers() {
  angleStep.moveTo(0);
  rotateStep.moveTo(0);
  if(angleStep.currentPosition() > 0){
    while(angleStep.currentPosition() > 0){
      angleStep.run();
    }
  }
  else{
    while(angleStep.currentPosition() < 0){
      angleStep.run();
    }
  }
  if(rotateStep.currentPosition() > 0){
    while(rotateStep.currentPosition() > 0){
      rotateStep.run();
    }
  }
  else{
    while(rotateStep.currentPosition() < 0){
      rotateStep.run();
    }
  }
}
