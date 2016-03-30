#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxProjectableImage::ofxProjectableImage(){
}
//--------------------------------------------------------------
void ofxProjectableImage::setup( ){
    imageDistance = 1;
    spatialWidth = 2.0;
    spatialHeight = 0.6;
    sphere.setup( 2 , 64 );
}
//--------------------------------------------------------------
void ofxProjectableImage::update( float theAngleTeta ){
    updateAngleTeta( theAngleTeta );
    updateSphereColor();
}
//--------------------------------------------------------------
void ofxProjectableImage::draw(int x , int y){
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
//--------------------------------------------------------------
void ofxProjectableImage::setImageInput( ofImage* &theInputImage ){
    imageInput = theInputImage;
}
//--------------------------------------------------------------
ofColor ofxProjectableImage::getSpherePixelColorFromIndexAndArc( int indexArc , int indexPixel ){
    if( indexArc >= sphere.arcs.size() || indexPixel >= sphere.arcs[ indexArc ]->leds.size() )
        return ofColor( 0,0,0 );
    return sphere.arcs[ indexArc ]->leds[ indexPixel ]->currentColor;
}

//private
//--------------------------------------------------------------
void ofxProjectableImage::updateAngleTeta( float theAngleTeta ){
    sphere.update( theAngleTeta );
}
//--------------------------------------------------------------
void ofxProjectableImage::updateSphereColor(){
    for( int a = 0 ; a < sphere.arcs.size() ; a ++ ){
        ofxNeoPixelLedArc* tempArc = sphere.arcs[ a ];
        for( int p = 0 ; p < tempArc->leds.size() ; p ++ ){
            float angleFi = tempArc->leds[p]->angleFi;
            float angleTeta = tempArc->leds[p]->angleTeta;
            float xp = tanf( angleFi ) * imageDistance / cosf( angleTeta );
            float yp = tanf( angleTeta ) * imageDistance;
            
            float deltaX = spatialWidth / imageInput->getWidth();
            float deltaY =  spatialHeight / imageInput->getHeight();
            int i = xp / deltaX + imageInput->getWidth() / 2;
            int j = yp / deltaY + imageInput->getHeight() / 2;
            if( i >= 0 && i < imageInput->getWidth() ){
                if( j>=0 && j < imageInput->getHeight() ){
                    ofColor temColor = imageInput->getColor(i, j);
                    tempArc->leds[p]->currentColor = temColor;
                }
                else
                    tempArc->leds[p]->currentColor = ofColor(0,0,0);
            }
            else
                tempArc->leds[p]->currentColor = ofColor(0,0,0);
        }
    }
}
//--------------------------------------------------------------