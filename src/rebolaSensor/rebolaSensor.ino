#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055();

void setup(void){
  Serial.begin(250000);
  delay( 2000 );
  Serial.println("Setting up sensor:");
  if(!bno.begin() ){
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  Serial.print("OK");
  delay(1000);
  bno.setExtCrystalUse(true);
}
//
//void loop(void){
//
//  bno.getEvent(&event);
//  
//  if( event.orientation.y == -12.2500 || event.orientation.z == -0.0625 ) 
//  return;
//    Serial.print( "a" );
//     Serial.print(event.orientation.x, 4);
//    Serial.print( " " );
//     Serial.print(event.orientation.y, 4);
//    Serial.print( " " );
//     Serial.print(event.orientation.z, 4);
//    Serial.print( "b" );
//    Serial.println();
//    //Serial.println();
//     delayMicroseconds(100);
//}

void loop(void){
    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    imu::Vector<3> vector = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    //if( vector.y() + 0.06 < 0.001 || vector.z() + 0.06 < 0.001 )
    //return;
    Serial.print( "a" );
    Serial.print( vector.x() );
    Serial.print( " " );
    Serial.print( vector.y() );
    Serial.print( " " );
    Serial.print( vector.z() );
    Serial.print( "b" );
    Serial.println();
    //Serial.println();
     delayMicroseconds(100);
}
