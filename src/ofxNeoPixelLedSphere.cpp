#include "ofxProjectableImage.h"
#include "ofxNeoPixelLedSphere.h"
#include "ofxNeoPixelLedArc.h"
//--------------------------------------------------------------
ofxNeoPixelLedSphere::ofxNeoPixelLedSphere(){
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::setup( int theNumArcs , int theNumLedsPerArc , float theRadius ){
    radius = theRadius;
    for( int a = 0 ; a < theNumArcs ; a ++ ){
        ofxNeoPixelLedArc* tempArc = new ofxNeoPixelLedArc();
        deltaAngleArcToArc = ( 2 * PI ) / theNumArcs;
        tempArc->setup( theNumLedsPerArc , a * deltaAngleArcToArc );
        tempArc->radius =radius;
        arcs.push_back( tempArc );
    }
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::update(float sphereAngleTeta ){
    for( int a = 0 ; a < arcs.size() ; a ++ )
        arcs[ a ]->update( sphereAngleTeta + a * deltaAngleArcToArc );
}
//--------------------------------------------------------------
void ofxNeoPixelLedSphere::projectImage( ofxProjectableImage* theProjectableImage ){
    projectableImage = theProjectableImage;
    for( int a = 0 ; a < arcs.size() ; a ++ )
        arcs[ a ]->projectImage( theProjectableImage  );
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
