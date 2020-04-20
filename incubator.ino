/* Egg Turner code using 28BYJ-48 with ULN2003 */

#include <AccelStepper.h>

//AccelStepper stepper1(FULLSTEP(4)orHALFSTEP(8),in1,in3,in2,in4);
AccelStepper stepper1(8, 2, 4, 3, 5);

int limit_1_status;
int limit_2_status;
int current_position;
int limit_1 = 6; // Limit switch 1
int limit_2 = 7; // Limit switch 2
int rotate_button = 9; // Manual rotate button
long t=3600000; //time interval; 3600000 ms = 1 hour
unsigned long current_time=0;
unsigned long prev_time=0;

void setup()
{
  Serial.begin(9600);
  
  stepper1.setMaxSpeed(200.0); // 200
  stepper1.setAcceleration(1000.0); // 1000
  //stepper1.setSpeed(200);
  //stepper1.moveTo(2038); //2038 for full rotation

  pinMode(limit_1,INPUT_PULLUP);
  pinMode(limit_2,INPUT_PULLUP);
  pinMode(rotate_button,INPUT_PULLUP);

  limit_1_status=digitalRead(limit_1);
  limit_2_status=digitalRead(limit_2);
  
  while(limit_1_status==HIGH) //Go to initial position of limit 1
  {
    stepper1.move(+30);
    stepper1.run();
    limit_1_status=digitalRead(limit_1);
  }
  current_position=1;
}

void loop()
{
  current_time=millis();
  if (current_time > (prev_time + t) || !digitalRead(rotate_button)) // If the desired time has passed or the rotate button is pressed 
  {
    switch (current_position)
    {
      case 1: //If the current position is 1, rotate to 2
        prev_time=millis(); //Store the time of rotation
        limit_2_status=digitalRead(limit_2);
        while(limit_2_status==HIGH) //Rotate until the limit switch 2 is pressed
        {
          stepper1.move(-30);
          stepper1.run();
          limit_2_status=digitalRead(limit_2);
        }
        current_position=2;
        break;

      case 2: //If the current position is 2, rotate to 1
        prev_time=millis(); //Store the time of rotation
        limit_1_status=digitalRead(limit_1);
        while(limit_1_status==HIGH) //Rotate until the limit switch 1 is pressed
        {
          stepper1.move(+30);
          stepper1.run();
          limit_1_status=digitalRead(limit_1);
        }
        current_position=1;
        break;
    }
  }
}
