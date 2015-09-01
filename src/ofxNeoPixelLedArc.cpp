#include "ofxNeoPixelLed.h"
#include "ofxNeoPixelLedArc.h"
#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxNeoPixelLedArc::ofxNeoPixelLedArc(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::setup( int theNumleds , float theRadius ){
    radius = theRadius;
    float deltaFi = ( PI / 2.0f ) / float( theNumleds );
    for( int l = 0 ; l < theNumleds ; l ++ ){
        ofxNeoPixelLed* tempLed = new ofxNeoPixelLed();
        tempLed->setup( deltaFi * ( l + 1 ) );
        leds.push_back( tempLed );
    }
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::update( float theAngleTeta ){
    float theNewAngle;
    float piDoubled = 2 * PI;
    if( theAngleTeta > piDoubled ){
        int howManyLaps = theAngleTeta / piDoubled;
        theNewAngle = theAngleTeta - howManyLaps * piDoubled;
    }
    else theNewAngle = theAngleTeta;
    
    for( int l = 0 ; l < leds.size() ; l ++ )
        leds[ l ]->update( theAngleTeta );
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::projectImage( ofxProjectableImage* theProjectableImage ){
    projectableImage = theProjectableImage;
    for( int l = 0 ; l < leds.size() ; l ++ )
        leds[ l ]->projectImage( projectableImage );
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::draw(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::mouseMoved(int x, int y ){
   
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::windowResized(int w, int h){
}
