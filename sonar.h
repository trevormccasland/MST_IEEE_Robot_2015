#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>

using namespace std;
 
//#define START_BUTTON 1
#define MODE_BUTTON 1
//#define STOP_BUTTON 9
#define START_LED 21
#define END_LED 6

#define TRIGLEFT 15
#define ECHOLEFT 3
#define ECHOFRONT 7
#define TRIGFRONT 0
#define ECHORIGHT 22
#define TRIGRIGHT 23
#define CAMLEFT 24
#define CAMRIGHT 25
#define READY 26
//#define BUTTON 7
#define MOVE 27
#define LEFT 28
#define RIGHT 29
typedef enum{
  CHECKWALLS,
  TAKEPICTURES,
  MOVEDIRECTION,
}t_robot_state;
void setup() {
	cout << "Start setup" << endl;
        wiringPiSetup();
       //pinMode(START_BUTTON, INPUT);
        pinMode(MODE_BUTTON, INPUT);
        pullUpDnControl(MODE_BUTTON, PUD_UP);
       // pinMode(STOP_BUTTON, INPUT);
        pinMode(TRIGLEFT, OUTPUT);
        pinMode(ECHOLEFT, INPUT);
        pinMode(TRIGFRONT, OUTPUT);
        pinMode(ECHOFRONT, INPUT);
        pinMode(TRIGRIGHT, OUTPUT);
        pinMode(ECHORIGHT, INPUT);
        pinMode(START_LED, OUTPUT);
        pinMode(END_LED, OUTPUT);
        pinMode(MOVE, OUTPUT);
        pinMode(LEFT, OUTPUT);
        pinMode(RIGHT, OUTPUT);
        pinMode(READY, INPUT);
        pinMode(CAMLEFT, OUTPUT);
        pinMode(CAMRIGHT, OUTPUT);
        //TRIG pin must start LOW
        digitalWrite(START_LED, HIGH);
        delay(30);
        digitalWrite(END_LED, LOW);
        delay(30);
        digitalWrite(MOVE, LOW);
        delay(30);
        digitalWrite(LEFT, LOW);
        delay(30);
        digitalWrite(RIGHT, LOW);
        delay(30);
        digitalWrite(READY, LOW);
        delay(30);
        digitalWrite(CAMLEFT, LOW);
        delay(30);
        digitalWrite(CAMRIGHT, LOW);
        delay(30);
        digitalWrite(TRIGLEFT, LOW);
        delay(30);
        digitalWrite(TRIGFRONT, LOW);
        delay(30);
        digitalWrite(TRIGRIGHT, LOW);
        delay(30);
}
int getCMLeft() {
        int distance=0;
        cout << "send left trigger pulse" << endl;
	do{
        //Send trig pulse	
        digitalWrite(TRIGLEFT, HIGH);
        delayMicroseconds(50);
        digitalWrite(TRIGLEFT, LOW);
	int count = 0;
	//cout << "wait for left  echo" << endl;
        //Wait for echo start
        while(digitalRead(ECHOLEFT) == LOW)
        {
         count++;
         if( count >= 3000 )
         {
          //Send trig pulse
          delayMicroseconds(50);
	  digitalWrite(TRIGLEFT, HIGH);
          delayMicroseconds(50);
          digitalWrite(TRIGLEFT, LOW);
          count = 0;
  	  //cout << "Reset Left" << endl;
         }
        }
        //cout << "echo recieved" << endl; 
        //Wait for echo end
        long startTime = micros();

        while(digitalRead(ECHOLEFT) == HIGH)
        {
//          cout<<"ECHO LEFT"<<endl;
        }
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        distance = travelTime/58;
// cout<<"Left distance: "<<distance<<endl;
        }while(distance==0);
        return distance;
}


int getCMRight() {
        //Send trig pulse
        int distance=0;
        do{
        digitalWrite(TRIGRIGHT, HIGH);
        delayMicroseconds(50);
        digitalWrite(TRIGRIGHT, LOW);
        
        int count = 0;
        //Wait for echo start
        while(digitalRead(ECHORIGHT) == LOW)
        {

         count++;

         if( count >= 3000 )
         {
          //Send trig pulse
          digitalWrite(TRIGRIGHT, HIGH);
          delayMicroseconds(50);
          digitalWrite(TRIGRIGHT, LOW);
          count = 0;
  //        cout << "Reset Right" << endl;
         }

        }


        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHORIGHT) == HIGH)
        {
//          cout<<"ECHO RIGHT"<<endl;

        }
        long travelTime = micros() - startTime;

        //Get distance in cm
        distance = travelTime / 58;
// cout<<"Right distance: "<<distance<<endl;
        }while(distance==0);
        return distance;
}

int getCMFront() {
        //Send trig pulse
        int distance=0;

        do{
        delayMicroseconds(50);
        digitalWrite(TRIGFRONT, HIGH);
        delayMicroseconds(50);
        digitalWrite(TRIGFRONT, LOW);
        int count = 0;
        //Wait for echo start
        while(digitalRead(ECHOFRONT) == LOW)
        {

         count++;

         if( count >= 3000 )
         {
          //Send trig pulse        
          digitalWrite(TRIGFRONT, HIGH);
          delayMicroseconds(50);
          digitalWrite(TRIGFRONT, LOW);
          count = 0;
//          cout << "Reset Front" << endl;
         }

        }


        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHOFRONT) == HIGH)
        {
//          cout<<"ECHO FRONT"<<endl;

        }
        long travelTime = micros() - startTime;

        //Get distance in cm
        distance = travelTime / 58;
// cout<<"Front distance: "<<distance<<endl;
        }while(distance==0);
        return distance;
}

void turnLeft()
{
  while(digitalRead(READY)==LOW)
  {
  }

  digitalWrite(LEFT, HIGH);
  delayMicroseconds(500);
  digitalWrite(LEFT, LOW);
  delayMicroseconds(500);
}

void move()
{
  while(digitalRead(READY)==LOW)
  {
  }

  digitalWrite(MOVE, HIGH);
  delayMicroseconds(500);
  digitalWrite(MOVE, LOW);
  delayMicroseconds(500);


}

void turnRight()
{
  while(digitalRead(READY)==LOW)
  {
  }

  digitalWrite(RIGHT, HIGH);
  delayMicroseconds(500);
  digitalWrite(RIGHT, LOW);
  delayMicroseconds(500);


}

bool listen()
{
  while(digitalRead(READY)==LOW);
  return true;
}

void camLeft()
{
  while(digitalRead(READY)==LOW)
  {
  }

  digitalWrite(CAMLEFT, HIGH);
  //digitalWrite(CAMLEFT, LOW);
  //delayMicroseconds(500);

}

void camRight()
{
  while(digitalRead(READY)==LOW)
  {
  }

  digitalWrite(CAMRIGHT, HIGH);
  //digitalWrite(CAMRIGHT, LOW);
  //delayMicroseconds(500);
}
