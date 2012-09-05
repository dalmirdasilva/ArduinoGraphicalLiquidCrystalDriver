#include <Glcd.h>
#include <GlcdStraight.h>

void setup() {
  Serial.begin(9600);

  Serial.println("Are you sure to continue? (y/n)");
  while (!Serial.available());
  while (Serial.read() != 'y');

  GlcdStraight glcdDriver;
    
  glcdDriver.init(Glcd::MODE_ON);
  glcdDriver.screen(0x00);

  for (int i = 0; i < 256; i++) {
    glcdDriver.screen(i);
    delay(100);
  }
  
  Serial.print("Finished at: ");
  Serial.println(millis());
}

void loop() {
}
