#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxProjectableImage::ofxProjectableImage(){
}
//--------------------------------------------------------------
void ofxProjectableImage::setup( ){
    image = new ofImage();
//    image->allocate( 255 , 255 , OF_IMAGE_COLOR );
//    for( int x = 0 ; x < 255 ; x ++ )
//        for( int y = 0 ; y < 255 ;  y++ )
//            image->setColor(x,y, ofColor( x , 0 , y)) ;
//    image->update();
    
//    
//    image->loadImage( "colors.jpg" );
//    image->rotate90(1);
    imageDistance = 1;
    spatialWidth = 2.0;
    spatialHeight = 0.6;
    sphere = new ofxNeoPixelLedSphere();
    sphere->setup( 2 , 64 , 1 );
}
//--------------------------------------------------------------
void ofxProjectableImage::setImage( ofImage* &theInputImage ){
    image = theInputImage;
}
//--------------------------------------------------------------
void ofxProjectableImage::updateAngleTeta( float theAngleTeta ){
    sphere->update( theAngleTeta );
}
//--------------------------------------------------------------
void ofxProjectableImage::updateSphereColor(){
    for( int a = 0 ; a < sphere->arcs.size() ; a ++ ){
        ofxNeoPixelLedArc* tempArc = sphere->arcs[ a ];
        for( int p = 0 ; p < tempArc->leds.size() ; p ++ ){
            float angleFi = tempArc->leds[p]->angleFi;
            float angleTeta = tempArc->leds[p]->angleTeta;
            float xp = tanf( angleFi ) * imageDistance / cosf( angleTeta );
            float yp = tanf( angleTeta ) * imageDistance;
            
            float deltaX = spatialWidth / image->width;
            float deltaY =  spatialHeight / image->height;
            int i = xp / deltaX + image->width / 2;
            int j = yp / deltaY + image->height / 2;
            if( i >= 0 && i < image->width ){
                if( j>=0 && j < image->height ){
                    ofColor temColor = image->getColor(i, j);
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
ofImage* ofxProjectableImage::getImage(){
    return image;
}
