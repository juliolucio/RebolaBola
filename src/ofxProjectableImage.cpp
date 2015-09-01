#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxProjectableImage::ofxProjectableImage(){
    image = new ofImage();
}
//--------------------------------------------------------------
void ofxProjectableImage::setup( ofImage* theImageToProject, ofVec3f thePosition , ofVec3f theNormal  ){
    image = theImageToProject;
    position = thePosition;
    normal =  theNormal;
}
//--------------------------------------------------------------
void ofxProjectableImage::update(){
}
//--------------------------------------------------------------
void ofxProjectableImage::draw(){
}
//--------------------------------------------------------------
void ofxProjectableImage::mouseMoved( int x, int y ){
}
//--------------------------------------------------------------
void ofxProjectableImage::mouseDragged( int x, int y, int button){
}
//--------------------------------------------------------------
void ofxProjectableImage::mousePressed( int x, int y, int button){
}
//--------------------------------------------------------------
void ofxProjectableImage::mouseReleased( int x , int y, int button){
}
//--------------------------------------------------------------
void ofxProjectableImage::windowResized( int w, int h){
}
