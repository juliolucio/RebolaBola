#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// angel of rotation from euler angles
//    angle = 2 * acos(c1c2c3 - s1s2s3)
//    x = s1 s2 c3 +c1 c2 s3
//    y = s1 c2 c3 + c1 s2 s3
//    z = c1 s2 c3 - s1 c2 s3
//
//    to normalise divide x,y and z by:
//
//    sqrt(x2 + y2 + z2) = sqrt((s1 s2 c3 +c1 c2 s3)2+(s1 c2 c3 + c1 s2 s3)2+(c1 s2 c3 - s1 c2 s3)2)
//
//where:
//
//    c1 = cos(heading / 2)
//    c2 = cos(attitude / 2)
//    c3 = cos(bank / 2)
//    s1 = sin(heading / 2)
//    s2 = sin(attitude / 2)
//    s3 = sin(bank / 2)


Adafruit_BNO055 bno = Adafruit_BNO055();
sensors_event_t event;
float heading;
float attitude;
float bank;

float c1;
float c2;
float c3;
float s1;
float s2;
float s3;
float angle;

void setup(void) {
  Serial.begin(115200);
  if (!bno.begin() ) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

void loop(void) {

  bno.getEvent(&event);

  //hack to filter sensor glitch for not been calibrated
  if ( event.orientation.y == -12.2500 || event.orientation.z == -0.0625 )
    return;

  heading   = 0.0174532925 * event.orientation.z;
  attitude  = 0.0174532925 * event.orientation.y;
  bank      = 0.0174532925 * event.orientation.x;

  c1 = cosf( heading / 2.0 );
  c2 = cosf( attitude / 2.0 );
  c3 = cosf( bank / 2.0 );
  s1 = sinf( heading / 2.0 );
  s2 = sinf( attitude / 2.0 );
  s3 = sinf( bank / 2.0);

  angle = 2 * acosf( c1 * c2 * c3 - s1 * s2 * s3 );
  Serial.print( "<" );
  Serial.print(angle, 4);
  Serial.print( ">" );
  Serial.println();

  delayMicroseconds(1);
}

//void loop(void){
//    // Possible vector values can be:
//    // - VECTOR_ACCELEROMETER - m/s^2
//    // - VECTOR_MAGNETOMETER  - uT
//    // - VECTOR_GYROSCOPE     - rad/s
//    // - VECTOR_EULER         - degrees
//    // - VECTOR_LINEARACCEL   - m/s^2
//    // - VECTOR_GRAVITY       - m/s^2
//    imu::Vector<3> vector = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
//    //if( vector.y() + 0.06 < 0.001 || vector.z() + 0.06 < 0.001 )
//    //return;
//    Serial.print( "a" );
//    Serial.print( vector.x() );
//    Serial.print( " " );
//    Serial.print( vector.y() );
//    Serial.print( " " );
//    Serial.print( vector.z() );
//    Serial.print( "b" );
//    Serial.println();
//    //Serial.println();
//     delayMicroseconds(100);
//}
