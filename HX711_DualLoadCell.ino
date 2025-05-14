#include <HX711.h>

// HX711 1
const int LOADCELL_DOUT_PIN_1 = 5;
const int LOADCELL_SCK_PIN_1 = 4;
HX711 scale1;

// HX711 2
const int LOADCELL_DOUT_PIN_2 = 19;
const int LOADCELL_SCK_PIN_2 = 18;
HX711 scale2;

void setup() {
  Serial.begin(115200);
  
  // Initialize HX711 1
  scale1.begin(LOADCELL_DOUT_PIN_1, LOADCELL_SCK_PIN_1);
  scale1.set_scale();
  scale1.tare();
  
  // Initialize HX711 2
  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale2.set_scale();
  scale2.tare();
  
  Serial.println("HX711 Dual Load Cell Reader");
  Serial.println("Time(ms),LoadCell1,LoadCell2");
}

void loop() {
  if (scale1.is_ready() && scale2.is_ready()) {
    long reading1 = scale1.read();
    long reading2 = scale2.read();
    
    Serial.print(millis());
    Serial.print(",");
    Serial.print(reading1);
    Serial.print(",");
    Serial.println(reading2);
  } else {
    Serial.println("HX711 not ready");
  }
  
  delay(100);
}