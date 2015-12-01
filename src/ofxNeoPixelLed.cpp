#include "ofxNeoPixelLed.h"
#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxNeoPixelLed::ofxNeoPixelLed(){
}
//--------------------------------------------------------------
void ofxNeoPixelLed::setup( float theAngleFi ){
    angleFi = theAngleFi;
}
//--------------------------------------------------------------
void ofxNeoPixelLed::update( float theAngleTeta ){
    angleTeta = theAngleTeta;
}

