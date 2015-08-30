#include "ofxNeoPixelLed.h"
#include "ofxNeoPixelLedArc.h"
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
    for( int l = 0 ; l < leds.size() ; l ++ ){
        leds[ l ]->update( theAngleTeta );
    }
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
