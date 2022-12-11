// M5StickC Library
#include <M5StickC.h>

// Define pins
#define LED 10
#define BTN 37
// Define pin enable/disable
#define ENABLED LOW
#define DISABLED HIGH

// var for reading pushbutton status
int btnState = 0;

// Acceleration
float accX =    0.0F;
float accY =    0.0F;
float accZ =    0.0F;
// Max Acceleration
float maxAccX = 0.0F;
float maxAccY = 0.0F;
float maxAccZ = 0.0F;

// Acceleration Vector
float accVectLength =     0.0F;
// Max Acceleration Vector
float maxAccVectLength =  0.0F;

// Axes Angular Velocity
float gyroX =   0.0F;
float gyroY =   0.0F;
float gyroZ =   0.0F;

// Axes Rotation
float pitch =   0.0F;
float roll  =   0.0F;
float yaw   =   0.0F;

// setup routine on startup (runs once):
void setup() {
  // initialize the M5StickC object
  M5.begin();
  // initialize the LED pin as an output
  pinMode(LED, OUTPUT);
  // turn the LED off (on by default)
  digitalWrite(LED, DISABLED);
  // initialize the button(A) pin as input
  pinMode(BTN, INPUT);
  // initialize the IMU
  M5.IMU.Init();

  // rotate the display to landscape
  M5.Lcd.setRotation(3);
  // fill the LCD screen black  
  M5.Lcd.fillScreen(BLACK);
  // set LCD text size
  M5.Lcd.setTextSize(1);
  
  // print "IMU MAX TEST" to LCD at cursor location
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("IMU MAX TEST");
  // print X,Y,Z Acceleration header to LCD at cursor location
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.println("  maxX    maxY    maxZ");
  // prnt Axes Rotation header to LCD at cursor location
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.println("  Pitch   Roll    Yaw");
}

// loop routine (runs repeatedly, forever):
void loop() {
  // update all IMU data
  updateIMU();

  // read the push state of btnA
  btnState = digitalRead(BTN);
  // if btnA is pressed:
  if(btnState == ENABLED) {
    // reset all acceleration/vector values
    accX = 0.0F;
    accY = 0.0F;
    accZ = 0.0F;
    maxAccX = 0.0F;
    maxAccY = 0.0F;
    maxAccZ = 0.0F;
    accVectLength = 0.0F;
    maxAccVectLength = 0.0F;

    // turn the LED off
    digitalWrite(LED, DISABLED);
  }

  // if the acceleration vector surpasses 1.5:
  if(maxAccVectLength > 1.5F) {
    // turn the LED on
    digitalWrite(LED, ENABLED);
  }

  // print all IMU data to the LCD screen
  printIMU();

  // wait 100ms
  delay(100);

  // update the M5StickC object
  M5.update();
}

// method to update all IMU values
void updateIMU() {
  // get IMU data
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);

  // set acceleration values positive
  accX = abs(accX);
  accY = abs(accY);
  accZ = abs(accZ);
  // if the updated acceleration value(s) are
  // larger than the current max acceleration
  // value(s), update them to the higher one
  //
  // otherwise, maintain the current maximum
  // acceleration value(s)
  maxAccX = max(maxAccX, accX);
  maxAccY = max(maxAccY, accY);
  maxAccZ = max(maxAccZ, accZ);

  // calculate acceleration vector
  accVectLength = sqrt(sq(accX) + sq(accY) + sq(accZ));
  // if the acceleration vector value increases, replace
  // the maximum acceleration vector value with the new
  // larger value
  //
  // otherwise, maintain the current maximum acceleration
  // vectory value
  maxAccVectLength = max(maxAccVectLength, accVectLength);
}

// method to print all IMU data to the LCD
void printIMU() {
  // print gyro data to LCD at cursor location
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
  // print gyro units to LCD at cursor location
  M5.Lcd.setCursor(140, 20);
  M5.Lcd.print("o/s");
  // print acceleration data to LCD at cursor location
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", maxAccX, maxAccY, maxAccZ);
  // print acceleration units to LCD at cursor location
  M5.Lcd.setCursor(140, 30);
  M5.Lcd.print("G");
  // print rotational data to LCD at cursor location
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);

  // print acceleration vector to LCD at cursor location
  M5.Lcd.setCursor(0, 70);
  M5.Lcd.printf("AccVector : %.2f C", maxAccVectLength);
}
