# define trig1 7
# define echo1 8
# define trig2 A3
# define echo2 A1


int s1 ; //IR Sensor 1 from left input will be taken into this variable
int s2 ; //IR Sensor 2 from left input will be taken into this variable
int s3 ; //IR Sensor 3 from left input will be taken into this variable
int s4 ; //IR Sensor 4 from left input will be taken into this variable
int s5 ; //IR Sensor 5 from left input will be taken into this variable


const int lmf = 6; //This variable has the pin for left motor forward
const int lmb = 5; //This variable has the pin for left motor backward
const int rmf = 10; //This variable has the pin for right motor forward
const int rmb = 11; //This variable has the pin for left motor backward
bool check = false; // This variable stores if the right wall has been encountered yet or not

const int wall_distance = 9; // The robot maintains this distance from the wall
int track = 0;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  Serial.begin(9600); // Used to setup the serial monitor
}

void forward()
{
  // Used to drive the robot forward
  analogWrite(rmf, 160);
  analogWrite(rmb, 0);
  analogWrite(lmf, 160);
  analogWrite(lmb, 0);
}

void backward()
{
  // Used to drive the robot backward
  analogWrite(rmf, 0);
  analogWrite(rmb, 160);
  analogWrite(lmf, 0);
  analogWrite(lmb, 160);
}

void brake()
{
  // Used to stop the robot
  analogWrite(rmf, 0);
  analogWrite(rmb, 0);
  analogWrite(lmf, 0);
  analogWrite(lmb, 0);
}

void right()
{
  // Used to drive the robot towards right
  analogWrite(rmf, 0);
  analogWrite(rmb, 160);
  analogWrite(lmf, 160);
  analogWrite(lmb, 0);
}

void left()
{
  // Used to drive the robot towards left
  analogWrite(rmf, 160);
  analogWrite(rmb, 0);
  analogWrite(lmf, 0);
  analogWrite(lmb, 160);
}

void slow_right()
{
  // Used to drive the robot towards right
  analogWrite(rmf, 0);
  analogWrite(rmb, 0);
  analogWrite(lmf, 160);
  analogWrite(lmb, 0);
}

void slow_left()
{
  // Used to drive the robot towards right
  analogWrite(rmf, 160);
  analogWrite(rmb, 0);
  analogWrite(lmf, 0);
  analogWrite(lmb, 0);
}

void followline() {
  // This code is used to make the robot follow the line
  s1 = digitalRead(2);
  s2 = digitalRead(3);
  s3 = digitalRead(4);
  s4 = digitalRead(12);
  s5 = digitalRead(13);

  int t1 = s1 + s2;
  int t2 = s4 + s5;

  if(t1 == 2 && t2 == 2 && s3 == 1)
  {
    digitalWrite(trig2, LOW);
    delayMicroseconds(2);
    digitalWrite(trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2, LOW);

    long rt2 = pulseIn(echo2, HIGH);
    
    int dist_right = rt2 * 0.034 / 2; //Distance from right wall calculated in cm

    if (dist_right < wall_distance+5)
    {
      // If right wall is detected once then execute the wall following code else execute the line following code
      check = true;
      return;
    }
  }

  if (t1 > t2 || (t1 == 2 && t2 == 2 && s3 == 1))
  {
    left();
  }
  else if (t1 <= t2 && s3 == 1)
  {
    forward();
  }
  else if (t1 < t2)
  {
    right();
  }

  else if (t1 == 0 && t2 == 0 && s3 == 0)
  {
    right();
  }
}
void wall() {
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);

  long rt1 = pulseIn(echo1, HIGH);

  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);

  long rt2 = pulseIn(echo2, HIGH);

  int dist_front = rt1 * 0.034 / 2; //Distance from front wall calculated in cm
  int dist_right = rt2 * 0.034 / 2; //Distance from right wall calculated in cm

  if (dist_front < wall_distance + 3) {
    // This code is for a hard turn in case a front obstacle is encountered
    track = 1;
    left();
    delay(300);
  }
  else if (dist_right < wall_distance) {
    slow_left();
  }
  else if (dist_right > wall_distance) {
    slow_right();
  }
  else
  {
    forward();
  }
}

void loop() {
  if (track == 1)
  {
    // This code is used to stop the robot after the wall ends and a black line is detected again
    s3 = digitalRead(4);
    if (s3 == 1)
    {
      brake();
      exit(0);
    }
  }
  if (!check) {
    followline();
  }
  else
  {
    wall();
  }
}
