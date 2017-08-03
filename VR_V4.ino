#include <Keyboard.h>
#include <Wire.h>
#include <Mouse.h>
//----------------------------------------//
  const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

  int minVal=265; int maxVal=402;               // Kiihtyvyysanturin juttuja

  double x; double y; double z;
//----------------------------------------//
  int joy_max = 563;                          // Joystickin herkkyyden säätö; neutraalina arvo on 513.
  int joy_min = 363;
//----------------------------------------//  
  int joy_button1 = 27;
  int joy_button2 = '8';
  int button11 = '0';
  int button12 = '9';    
  int button21 = 32;                           // button11 = 1. painikin ilman modifieria, button12 = 1. painike modifierin kanssa.
  int button22 = 'h';
  int button31 = 'e';
  int button32 = 'r';                           // Näppäintien yms. muuttujia
  int up = 'w';
  int left = 'a';
  int down = 's';
  int right = 'd';                              // Keyboard muuttujat voidaan tarvittaessa muuttaa hiiren komennoiksi Mouse.press komennolla
  int gyro_forward1 = '3';//ohjaimen kääntö eteen
  int gyro_forward2 = 'p';//ohjaimen kääntö eteen kun 4. painike pohjassa
  int gyro_back1 = '2';//ohjaimen kääntö taa
  int gyro_back2 = '3';//ohjaimen kääntö taa kun 4. painike pohjassa
  int gyro_left1 = '1';//ohjaimen kääntö vasemmalle
  int gyro_left2 = 'q';//ohjaimen kääntö vasemmalle kun 4.painike pohjassa
  int gyro_right1 = '4';//ohjaimen kääntö oikealle
  int gyro_right2 = 'm';//ohjaimen kääntö oikealle kun 4. painike pohjassa
  int xposition;
  int yposition;
//----------------------------------------//
  int analogPin_x = 0;
  int analogPin_y = 1;
  int joy_SW = 6;
  int nappula1 = 16;                            // Pinnien määritykset
  int nappula2 = 7;
  int nappula3 = 8;
  int nappula4 = 9;
//----------------------------------------//
  int xval = 0;
  int yval = 0;
  int joy_painike = 0;
  int nappipainike1 = 0;                        // Alkuarvot muuttujille
  int nappipainike2 = 0;
  int nappipainike3 = 0;
  int nappipainike4 = 0;
  int x1 = 0;
  int y1 = 0;
  int z1 = 0;
//----------------------------------------//
  void setup(){
  Serial.begin(9600);
  Wire.begin(); 
  Mouse.begin();
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B); Wire.write(0);              // Lisää kiihtyvyysanturin juttuja
  Wire.endTransmission(true);
//----------------------------------------//
  pinMode(joy_SW, INPUT_PULLUP);
  pinMode(nappula1, INPUT_PULLUP); 
  pinMode(nappula2, INPUT_PULLUP);              // Pinmodet
  pinMode(nappula3, INPUT_PULLUP);
  pinMode(nappula4, INPUT_PULLUP); 
}
//----------------------------------------//
void loop(){
  arvojen_lukeminen();
  kiihtyvyysanturi();
  painikekomennot();
  //gyro_komennot();
  mouseposition();
}
//----------------------------------------//
void painikekomennot(){
    nappifunktio(joy_painike, joy_button1, joy_button2);//joystickin painike
    nappifunktio_hiiri();//1.painike
    nappifunktio(nappipainike2, button21, button22);//2.painike
    nappifunktio(nappipainike3, button31, button32);//3.painike
}
//----------------------------------------//
int hand_pos(int x_k, int y_k, int z_k, int e_r, int button){
  if ((x_k - e_r) < x and (x_k + e_r) > x){
    x1 = 1;
  }
    else{
      x1 = 0;
    }
  
  if ((y_k - e_r) < y and (y_k + e_r) > y){
    y1 = 1;
  }
    else{
      y1 = 0;
    }
  
  if ((z_k - e_r) < z and (z_k + e_r) > z){
    z1 = 1;
  }
    else{
      z1 = 0;
    }

  if (x1 == 1 and y1 == 1 and z1 == 1){
    Keyboard.press(button);
    x1 = 0;
    y1 = 0;
    z1 = 0;
  }
    else{
      Keyboard.release(button);
    }
}
//----------------------------------------//
/*void gyro_komennot(){
  hand_pos();
  hand_pos();
  hand_pos();
  hand_pos();
  hand_pos();
}*/
//----------------------------------------//
void nappifunktio(int painike, int b1, int b2){
  if (painike == LOW and nappipainike4 == HIGH){                    
        Keyboard.press(b1); 
      }                              
      else if (painike == LOW and nappipainike4 == LOW){
        Keyboard.press(b2);
       }
      else{
        Keyboard.release(b1);
        Keyboard.release(b2);
      }
}
//----------------------------------------//
void nappifunktio_hiiri(){
    if (nappipainike1 == LOW and nappipainike4 == HIGH){                    
        //Mouse.press(MOUSE_LEFT); 
      }                              
      else if (nappipainike1 == LOW and nappipainike4 == LOW){
        Mouse.press(MOUSE_RIGHT);
       }
      else{
        Mouse.release(MOUSE_RIGHT);
        Mouse.release(MOUSE_LEFT);
      }
}
//----------------------------------------//
void mouseposition(){
  xposition = map(analogRead(analogPin_x), 0, 1023, 0, 1920);
  yposition = map(analogRead(analogPin_y), 0, 1023, 0, 1080);
  Mouse.move(xposition, yposition, 0); 
}
//----------------------------------------// 
void kiihtyvyysanturi(){
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B); Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,true); 
      AcX=Wire.read()<<8|Wire.read(); 
      AcY=Wire.read()<<8|Wire.read(); 
      AcZ=Wire.read()<<8|Wire.read(); 
      int xAng = map(AcX,minVal,maxVal,-90,90);                       // Kiihtyvyysanturin juttuja
      int yAng = map(AcY,minVal,maxVal,-90,90); 
      int zAng = map(AcZ,minVal,maxVal,-90,90);      
      x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
      Serial.print("AngleX= "); Serial.println(x);
      Serial.print("AngleY= "); Serial.println(y);
      Serial.print("AngleZ= "); Serial.println(z); Serial.println("-----------------------------------------"); delay(1);
}
//----------------------------------------//
void arvojen_lukeminen(){
  joy_painike = digitalRead(joy_SW);
  nappipainike1 = digitalRead(nappula1);
  nappipainike2 = digitalRead(nappula2);
  nappipainike3 = digitalRead(nappula3);                // Määrityksiä
  nappipainike4 = digitalRead(nappula4);
  xval = analogRead(analogPin_x);
  yval = analogRead(analogPin_y);
}
