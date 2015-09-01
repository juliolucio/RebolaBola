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
    //todo:calculate the pixell color with the image position and sphere pixel position
}
//--------------------------------------------------------------
void ofxNeoPixelLed::projectImage( ofxProjectableImage* theProjectableImage ){
    projectableImage = theProjectableImage;
}
//--------------------------------------------------------------
void ofxNeoPixelLed::draw(){
}
//--------------------------------------------------------------
void ofxNeoPixelLed::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void ofxNeoPixelLed::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLed::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLed::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLed::windowResized(int w, int h){
}
