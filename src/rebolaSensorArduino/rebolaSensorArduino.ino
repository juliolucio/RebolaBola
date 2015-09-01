#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (1)
Adafruit_BNO055 bno = Adafruit_BNO055();


void setup(void){
  Serial.begin(115200);
  if(!bno.begin() ){
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

void loop(void){
    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    imu::Vector<3> vector = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    Serial.print( "a" );
    Serial.print( vector.x() );
    Serial.print( " " );
    Serial.print( vector.y() );
    Serial.print( " " );
    Serial.print( vector.z() );
    Serial.print( "b" );
    Serial.println();
    //Serial.println();
     delay(BNO055_SAMPLERATE_DELAY_MS);
}
