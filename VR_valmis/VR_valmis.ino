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
  int y_left = 140;
  int y_right = 60;
  int z_up = 300;                               // Kulmien arvojen muuttujia. Kulmat näkee Sarjamonitorilta.
  int z_upcap = 340;
  int z_down = 20;
  int z_downcap = 100;
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
  int gyro_forward1 = '3';
  int gyro_forward2 = 'p';
  int gyro_back1 = '2';
  int gyro_back2 = '3';
  int gyro_left1 = '1';
  int gyro_left2 = 'q';
  int gyro_right1 = '4';
  int gyro_right2 = 'm';
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
//----------------------------------------//
  void setup()
{
  Serial.begin(9600);
//----------------------------------------//  
  Wire.begin(); 
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
void loop()
{

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
//----------------------------------------//  
  joy_painike = digitalRead(joy_SW);
  nappipainike1 = digitalRead(nappula1);
  nappipainike2 = digitalRead(nappula2);
  nappipainike3 = digitalRead(nappula3);                // Määrityksiä
  nappipainike4 = digitalRead(nappula4);
  xval = analogRead(analogPin_x);
  yval = analogRead(analogPin_y);
//----------------------------------------//
  if (z > z_up and z < z_upcap and nappipainike4 == HIGH){
    Keyboard.press(gyro_forward1);
    }                                                                         // Liikekontrolleja modifierilla
    else if (z > z_up and z < z_upcap and nappipainike4 == LOW){
      Keyboard.press(gyro_forward2);
      }
    else{
      Keyboard.release(gyro_forward1);
      Keyboard.release(gyro_forward2);
    }
//----------------------------------------//  
  if (z > z_down and z < z_downcap and nappipainike4 == HIGH){
    Keyboard.press(gyro_back1);
    }
    else if (z > z_down and z < z_downcap and nappipainike4 == LOW){                               
      Keyboard.press(gyro_back2);
      }
    else{
      Keyboard.release(gyro_back1);
      Keyboard.release(gyro_back2);
    }
//----------------------------------------//
  if (y > y_left and nappipainike4 == HIGH){
    Keyboard.press(gyro_left1);
    }
    else if (y > y_left and nappipainike4 == LOW){
      Keyboard.press(gyro_left2);
      }
    else{
      Keyboard.release(gyro_left1);
      Keyboard.release(gyro_left2);
    }
//----------------------------------------// 
    if (y < y_right and nappipainike4 == HIGH){
      Keyboard.press(gyro_right1);
     }
      else if (y < y_right and nappipainike4 == LOW){
        Keyboard.press(gyro_right2);
        }
      else{
        Keyboard.release(gyro_right1);
        Keyboard.release(gyro_right2);
      }
           
//----------------------------------------//    
   nappifunktio(joy_painike, joy_button1, joy_button2);
//----------------------------------------//    
    if (nappipainike1 == LOW and nappipainike4 == HIGH){                    
        Mouse.press(MOUSE_LEFT); 
      }                              
      else if (nappipainike1 == LOW and nappipainike4 == LOW){
        Mouse.press(MOUSE_RIGHT);
       }
      else{
        Mouse.release(MOUSE_RIGHT);
        Mouse.release(MOUSE_LEFT);
      }
//----------------------------------------//
  nappifunktio(nappipainike2, button21, button22);
//----------------------------------------//
  nappifunktio(nappipainike3, button31, button32);
//----------------------------------------//      
    if (xval > joy_max ){                           
      Keyboard.press(right); 
    }
      else{                                               // Joystickin määritys
        Keyboard.release(right); 
       }
//----------------------------------------//    
    if (xval < joy_min){                          
      Keyboard.press(left); 
      }
      else{
        Keyboard.release(left); 
       }
//----------------------------------------//    
    if (yval > joy_max){                            
      Keyboard.press(down);
      }
      else{
        Keyboard.release(down); 
       }
//----------------------------------------//    
    if (yval < joy_min){                            
      Keyboard.press(up); 
      }
      else{
        Keyboard.release(up); 
        }
}
//----------------------------------------// 
int nappifunktio(int painike, int b1, int b2){
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
  
