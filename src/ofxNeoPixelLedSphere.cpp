#include "ofxProjectableImage.h"
#include "ofxNeoPixelLedSphere.h"
#include "ofxNeoPixelLedArc.h"
//--------------------------------------------------------------
ofxNeoPixelLedSphere::ofxNeoPixelLedSphere(){
}
//--------------------------------------------------------------
ofxNeoPixelLedSphere::~ofxNeoPixelLedSphere(){
    clear();
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::setup( int theNumArcs , int theNumLedsPerArc  ){
    deltaAngleArcToArc = ( 2 * PI ) / theNumArcs;
    for( int a = 0 ; a < theNumArcs ; a ++ ){
        ofxNeoPixelLedArc* tempArc = new ofxNeoPixelLedArc();
        tempArc->setup( theNumLedsPerArc );
        arcs.push_back( tempArc );
    }
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::update(float sphereAngleTeta ){
    for( int a = 0 ; a < arcs.size() ; a ++ )
        arcs[ a ]->update( sphereAngleTeta + a * deltaAngleArcToArc );
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
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::clear(){
    for( int a = 0 ; a < arcs.size() ; a ++ )
        delete arcs[a];
    arcs.clear();
}