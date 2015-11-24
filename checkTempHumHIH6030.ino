
/* This reads temp/ humidity measures from a honeywell i2c hih6xxx sensor using
 * a TeensyLC mcu, so the i2c library is specific to this board.
 *
 * */

#include <i2c_t3.h>


void setup() {
  Serial.begin( 9600 );
  Wire.begin( I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_100 );
}

uint8_t v = 0;
uint8_t count = 0;

uint8_t target = 0x27;


uint8_t buffer[4];

float t, h;

void loop() {

  int i;
  int rc;

  // and empty i2c transaction triggers the measurement cycle
  Wire.beginTransmission( target ); 
  rc = Wire.endTransmission();
  
  if (rc != 0){
    Serial.print("Warning, start measurement failed\n"); 
  }

  // check the datasheet for how long it takes, but 10ms seems to be enough
  delay(10);
  
  Serial.print(count++);  
  Serial.print(": ");

  // should send back 4 bytes (2 bytes for temp, 2 for humidity)
  int avail = Wire.requestFrom( target, 4 );
  
  // some chips may have only one sensor and will send back 2 bytes only
  // should handle that case as well.

  if (avail == 4){
     for (i = 0; i < 4; i++){
       buffer[i] = Wire.readByte();
     }

     // ugly maths, check the datasheet for details (page 6)
     h = ((((uint16_t) (buffer[0] & 0x3f)) << 8) | buffer[1]) / 163.820;
     t = ((((((uint16_t) buffer[2]) << 8) | buffer[3]) >> 2) / 16382.0) * 165 - 40;
     
     // teensy can print floats. some arduinos cannot. you have been warned.
     Serial.print("temp: ");
     Serial.print(t);
     Serial.print(" ");
     Serial.print("humid: ");
     Serial.print(h);
     

       
  }else{
     Serial.print("request failed with ");
     Serial.print(avail);

  }
  Serial.print("\n");  
  
  

  
  
  delay(100);
}

