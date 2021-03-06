#include <Servo.h>

#define TURN_PIN 5
#define GO_PIN 3
#define MIN_TURN 700
#define MAX_TURN 2000
#define STOP_SPEED 1500
#define MAX_SPEED_GO 2000
#define MAX_SPEED_BACK 1000
Servo turn;
Servo sgo;
char command;
String string;
int svangle = 0;
int slideBarValue = 50;
int index = 0;
String aCmd;

int speeds = STOP_SPEED;
int gear = 0;

void carGo(int st){
  sgo.writeMicroseconds(st);
  delay(10);
}

void setup()
{
  //Serial.begin( 9600 );//115200
  Serial.begin( 115200 );
  pinMode(LED_BUILTIN, OUTPUT);
  turn.attach(TURN_PIN);
  svangle = map(slideBarValue, 0, 100, MIN_TURN, MAX_TURN);
  turn.writeMicroseconds(svangle);

  sgo.attach(GO_PIN);
  sgo.writeMicroseconds(speeds);
  //Serial.println("Setup done!");
}

void loop()
{
  string = "";
  while(Serial.available() > 0)
  {
    command = ((byte)Serial.read());
    if(command == ':')
    {
      break;
    }
    else
    {
      string += command;
    }
    delay(1);
  }
  //if(string != "")  Serial.println(string);
  
  while( string.length() >= 3 ){
      aCmd = string.substring(0, 3);
      string = string.substring(3);
      //Serial.println(" " + aCmd);

      index = aCmd.lastIndexOf("T");
      if( aCmd == "GOO"  ){
        // Move the car 
        carGo(MAX_SPEED_GO);
      } else if( aCmd == "STG" ){
        carGo(STOP_SPEED);
        // Stop the car
      } else if( aCmd == "BAC" ){
        // Move the car back
        carGo(MAX_SPEED_BACK);
      } else if( aCmd == "STB" ){
        // Stop the car
        carGo(STOP_SPEED);
      } else if( index == 0 ){
        // Turn left/right: cmd = "T<value from 0 to 100>"
          slideBarValue = aCmd.substring(index+1).toInt();
          //Serial.println(slideBarValue );
          if( slideBarValue > 0 ){
            //turn.attach(TURN_PIN);
            svangle = map(slideBarValue, 0, 100, MIN_TURN, MAX_TURN);
            turn.writeMicroseconds(svangle);
          }
      } else if ( aCmd.lastIndexOf("S") == 0 ){
        speeds = aCmd.substring(1).toInt();
        if( speeds > 0 ){
          speeds -= 15;
          if( gear == 3 ){
            sgo.writeMicroseconds( map(speeds, 0, 100, STOP_SPEED, MAX_SPEED_GO) );
          } else if( gear == 1 ){
            sgo.writeMicroseconds( map(speeds, 0, 100, STOP_SPEED, MAX_SPEED_BACK) );
          }
          delay(10);
        }
      } else if ( aCmd.lastIndexOf("G") == 0 ){
        gear = aCmd.substring(1).toInt();
      }
  }
}

