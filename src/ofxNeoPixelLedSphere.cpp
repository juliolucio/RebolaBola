#include "ofxNeoPixelLedSphere.h"
#include "ofxNeoPixelLedArc.h"
//--------------------------------------------------------------
ofxNeoPixelLedSphere::ofxNeoPixelLedSphere(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::setup( int theNumArcs , int theNumLedsPerArc ){
    for( int a = 0 ; a < theNumArcs ; a ++ ){
        ofxNeoPixelLedArc* tempArc = new ofxNeoPixelLedArc();
        tempArc->setup( theNumLedsPerArc , a * (( 2 * PI ) / theNumArcs) );
        arcs.push_back( tempArc );
    }
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::update(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::draw(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::mouseMoved( int x, int y ){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::mouseDragged( int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::mousePressed( int x, int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::mouseReleased( int x , int y, int button){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::windowResized( int w, int h){
}
