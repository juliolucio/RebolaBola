#include "ofxNeoPixelLed.h"
#include "ofxNeoPixelLedArc.h"
#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxNeoPixelLedArc::ofxNeoPixelLedArc(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::setup( int theNumleds , float theRadius ){
    radius = theRadius;
    float deltaFi =  PI / float( theNumleds + 2 );
    float piMedios = PI / 2.0;
    for( int l = 0 ; l < theNumleds ; l ++ ){
        ofxNeoPixelLed* tempLed = new ofxNeoPixelLed();
        tempLed->setup( deltaFi * ( l + 1 ) - piMedios );
        leds.push_back( tempLed );
    }
}
//--------------------------------------------------------------
void ofxNeoPixelLedArc::update( float theAngleTeta ){
    float piDoubled = 2 * PI;
    theAngleTeta = fmod( theAngleTeta , piDoubled );
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
