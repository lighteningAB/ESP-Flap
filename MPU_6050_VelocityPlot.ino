#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
float velocityX = 0, velocityY = 0, velocityZ = 0;
unsigned long lastTime = 0;
const float dt = 0.01; // 10ms sampling

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while(1);
  }
  
  Serial.println("Time(ms),VelX(m/s),VelY(m/s),VelZ(m/s)");
  lastTime = millis();
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Convert to m/s² (MPU6050 scale: ±2g = ±16384 LSB)
  float accelX = (ax / 16384.0) * 9.81;
  float accelY = (ay / 16384.0) * 9.81;
  float accelZ = (az / 16384.0) * 9.81;
  
  // Integrate acceleration to get velocity
  velocityX += accelX * dt;
  velocityY += accelY * dt;
  velocityZ += accelZ * dt;
  
  // Output for plotting
  Serial.print(millis());
  Serial.print(",");
  Serial.print(velocityX);
  Serial.print(",");
  Serial.print(velocityY);
  Serial.print(",");
  Serial.println(velocityZ);
  
  delay(10);
}