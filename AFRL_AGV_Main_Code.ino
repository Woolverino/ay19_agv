// Robotics with the BOE Shield - ForwardThreeSeconds
// Make the BOE Shield-Bot roll forward for three seconds, then stop.

#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;



// Constants and Variables

const float darkroom = .5;
int inDarkRoom = 0; // if 0 no, 1 yes
const int pingPin = 5;
const int pingPin2 = 7;
int obstacleDistance = 12;
int cur_left_inches = obstacleDistance;
int cur_right_inches = obstacleDistance;

long duration1, duration2;


  int myservo1_stop = 1500;
  int myservo1_forward = 1600;
  int myservo1_back = 1400;
  
  int myservo2_stop = 1345;
  int myservo2_forward = 1445 ;
  int myservo2_back = 1245;

 
////////////////////////////////////////////


// Functions 

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

// IR Object Detection Function

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  

float volts(int adPin)                       // Measures volts at adPin 
{                                            // Returns floating point voltage
 return float(analogRead(adPin)) * 5.0 / 1024.0; 
}    




// The PING is triggered by a HIGH pulse of 2 or more microseconds.
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
long leftPing() {
  long duration;
  int inches;
  pinMode(pingPin, OUTPUT);
  
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(pingPin, LOW);
  
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  inches = microsecondsToInches(duration);
  if (inches<obstacleDistance)
  {
    return 1;  
  }
  else 
  {
    return 0;
  }
}


long rightPing() {

  long duration;
  int inches;
  pinMode(pingPin2, OUTPUT);
  
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(pingPin2, LOW);
  
  pinMode(pingPin2, INPUT);
  duration = pulseIn(pingPin2, HIGH);
  inches = microsecondsToInches(duration);
  if (inches<obstacleDistance)
  {
    return 1;  
  }
  else 
  {
    return 0;
  }
  //return inches;
  
}

//Left forward=112; stop=1555;back=75
//Right forward=100; stop=1450; back=68
//turn left: left=85 ; right=100 
//turn right: left=112 ; right=78

void moveLeft() {
  servoLeft.write(80);
  servoRight.write(120);

  delay(300);
  
}

void moveRight() {
  servoLeft.write(120);
  servoRight.write(85);
  delay(300);
  
}


void moveForward() {

  
  servoLeft.write(105);
  servoRight.write(120);
  delay(500);
  
  //servoStop();
  
}

void moveBackward() {
   servoLeft.write(80);
  servoRight.write(95);
  delay(500);
}


void servoStop() {
   servoLeft.write(105);
   servoRight.write(105);
   delay(500);
}

void mainMove(int left, int right, int left_ir, int right_ir) {
  int leftHot = 0;
  int rightHot = 0;
  

  
  
  if (left || left_ir) {
    
    leftHot = 1; 
  }

  if (right || right_ir) {
    rightHot = 1; 
  }

  Serial.println("WHO IS HOT?");
  Serial.println(leftHot);
  Serial.println(rightHot);

  if (leftHot && rightHot) {
    // straight ahead obstacle
    Serial.println("STRAIGHT AHEAD OBSTACLE, GOIN BACK AND TakIn a ChAnCe, FeElin' LucKy!");
    moveBackward();
    int rando = random(1,3);
    /*
    if (rando == 2) {
      Serial.println("RIGHT IT IS!");
      moveRight();
    }
     else {
      Serial.println("LEFT IS IS!");
      moveLeft();
     }
     */
     moveLeft();
    }
  
  
  else if (leftHot && !rightHot) {
    // obstacle left
     Serial.println("OBSTACLE On The LEFT, GOIN BACK and RIGHT!");
    moveBackward();
    moveRight();
  }
  else if (!leftHot && rightHot) {
    // obstacle right
    Serial.println("OBSTACLE On The RIGHT, GOIN BACK and LEFT!");
    moveBackward();
    moveLeft();
  }
  else {
    Serial.println("NO OBSTACLE GOING FORWARD!");
    moveForward();
  }
}

void lightseeker() {
  float v1;
  float v2;

  servoStop();
  moveForward();

   // check left
   moveLeft(); 
   v1 = volts(A4);
   Serial.print(v1);
   Serial.println("\n");

    // recenter
    moveRight();
    
  // check right  
  moveRight();
  v2 = volts(A4);
  Serial.print(v2);
  Serial.println("\n");
  moveLeft(); 

  // which one better??
  if (v1 > v2) {
    Serial.println("GOING LEFT BOYS");
    moveLeft();  
  }
  else {
    Serial.println("GOING RIGHT BOYS");
    moveRight();
  }

  lightseeker();
}

/////////////////////////////////////////////////////
 
void setup()                                 // Built-in initialization block
{ 
  servoLeft.attach(13);                      // Attach left signal to P13 
  servoRight.attach(12);                     // Attach right signal to P12

  pinMode(3, INPUT);  pinMode(2, OUTPUT);  
  pinMode(4, INPUT);  pinMode(5, OUTPUT); 
   
  // initialize serial communication:
  Serial.begin(9600);
  // Full speed forward
  //servoLeft.write(112);         // Left wheel counterclockwise
  //servoRight.write(80);        // Right wheel clockwise
  //delay(1250);

  //servoLeft.writeMicroseconds(1555);         // Left wheel counterclockwise
  //servoRight.writeMicroseconds(1450);
  //delay(500);
  
  
}  
 
void loop()                                  // Main loop auto-repeats
{  
  
   // get all sensor telemetry
  Serial.println("ROOM ROOOOOM!");
  int left = leftPing();
  int right = rightPing();
  int left_ir = irDetect(2, 3, 38000);
  int right_ir = irDetect(4, 5, 38000);
  float voltage = volts(A4);
   // update all telemetry variables
  Serial.print(left);
  Serial.print(right);
  Serial.print(left_ir);
  Serial.print(right_ir);
  Serial.print("\n");
  Serial.print(voltage);
  Serial.print("\n");

  delay(100);


  // check if in light room
  if ((voltage < darkroom) && (inDarkRoom == 0)) {
    inDarkRoom = 1;
    Serial.println("In ThE DaRk RoOm");
    lightseeker();
  }
  // not in light room, main manuever
  mainMove(left, right, left_ir, right_ir);
  
  
  
  delay(50);
}
//Left forward=112; stop=1555;back=75
//Right forward=100; stop=1450; back=68
//turn left: left=85 ; right=100 
//turn right: left=112 ; right=78

