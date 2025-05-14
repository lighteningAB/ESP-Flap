#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int SERVO_MIN = 184;  // 900us at 50Hz
const int SERVO_MAX = 430;  // 2100us at 50Hz
const int SERVO_CENTER = 307; // 1500us at 50Hz

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  
  pwm.begin();
  pwm.setPWMFreq(50); // 50Hz for servos
  
  Serial.println("Servo Control - Enter angle (-50 to +50):");
  Serial.println("Pulse widths: -50°=1000us, 0°=1500us, +50°=2000us");
}

void loop() {
  if (Serial.available()) {
    int angle = Serial.parseInt();
    if (angle >= -50 && angle <= 50) {
      int pulseWidth = map(angle, -50, 50, 205, 410); // 1000us to 2000us
      pwm.setPWM(0, 0, pulseWidth);
      
      Serial.print("Angle: ");
      Serial.print(angle);
      Serial.print("° -> Pulse: ");
      Serial.print(1000 + (angle + 50) * 10);
      Serial.println("us");
    } else {
      Serial.println("Angle out of range (-50 to +50)");
    }
  }
}