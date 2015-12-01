#include "ofxScreenCapture.h"

extern "C" {
#include "macGlutfix.h"
}
//--------------------------------------------------------------
ofxScreenCapture::ofxScreenCapture(){
}
//--------------------------------------------------------------
void ofxScreenCapture::setup( bool theRetina ){
    retina = theRetina;
    int retinalFactor = 1;
    if( retina  )
        retinalFactor = 2;
    
    backgroundImage.allocate( retinalFactor * ofGetWidth(), retinalFactor * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
}
//--------------------------------------------------------------
void ofxScreenCapture::update(){
    unsigned char * pixelsBackground = backgroundImage.getPixels();
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int retinalFactor = 1;
    if( retina )
        retinalFactor = 2;
    
    pixelsBackground = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , ofGetWidth()  , ofGetHeight() );
    for( int i = 0; i < retinalFactor * retinalFactor * ofGetWidth() * ofGetHeight() ; i++ ){
        r = pixelsBackground[ 4 * i + 1 ];
        g = pixelsBackground[ 4 * i + 2 ];
        b = pixelsBackground[ 4 * i + 3 ];
        a = pixelsBackground[ 4 * i + 0 ];
        
        pixelsBackground[ 4 * i + 0 ] = r;
        pixelsBackground[ 4 * i + 1 ] = g;
        pixelsBackground[ 4 * i + 2 ] = b;
        pixelsBackground[ 4 * i + 3 ] = a;
    }
    backgroundImage.setFromPixels( pixelsBackground , retinalFactor * ofGetWidth(), retinalFactor * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
    backgroundImage.update();
    
}
//--------------------------------------------------------------
void ofxScreenCapture::draw(){
    backgroundImage.draw( 0,0, ofGetWidth() , ofGetHeight() );
}
//--------------------------------------------------------------
void ofxScreenCapture::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void ofxScreenCapture::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxScreenCapture::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxScreenCapture::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxScreenCapture::windowResized(int w, int h){
    backgroundImage.clear();
    setup( retina );
}
