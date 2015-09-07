#include "ofxScreenSelector.h"

extern "C" {
#include "macGlutfix.h"
}

//--------------------------------------------------------------
ofxScreenSelector::ofxScreenSelector(){
}
//--------------------------------------------------------------
void ofxScreenSelector::setup( bool theRetina , ofPoint thePosition , ofPoint theSize ){
    marker.setup();
    marker.setPosition( thePosition );
    marker.setSize( theSize );
    
    isRetinaScreen = theRetina;
    retinalFactor = 1;
    if( isRetinaScreen  )
        retinalFactor = 2;
    
    selectionFormBackgoundImage = new ofImage();
    selectionFormBackgoundImage->allocate( retinalFactor * marker.getSize().x, retinalFactor *  marker.getSize().y, OF_IMAGE_COLOR_ALPHA);
}
//--------------------------------------------------------------
void ofxScreenSelector::update(){
    unsigned char* pixelsSelected = selectionFormBackgoundImage->getPixels();
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
    pixelsSelected = pixelsBelowWindow( ofGetWindowPositionX() + marker.getPosition().x , ofGetWindowPositionY() + marker.getPosition().y , marker.getSize().x , marker.getSize().y );
    
    for (int i = 0; i < retinalFactor  * retinalFactor * marker.getSize().x * marker.getSize().y; i++){
        r = pixelsSelected[ 4 * i + 1 ];
        g = pixelsSelected[ 4 * i + 2 ];
        b = pixelsSelected[ 4 * i + 3 ];
        a = pixelsSelected[ 4 * i + 0 ];
        
        pixelsSelected[ 4 * i + 0 ] = r;
        pixelsSelected[ 4 * i + 1 ] = g;
        pixelsSelected[ 4 * i + 2 ] = b;
        pixelsSelected[ 4 * i + 3 ] = a;
    }
    selectionFormBackgoundImage->setFromPixels(pixelsSelected,  retinalFactor * marker.getSize().x,  retinalFactor * marker.getSize().y, OF_IMAGE_COLOR_ALPHA );
    selectionFormBackgoundImage->update();
}
//--------------------------------------------------------------
void ofxScreenSelector::draw(){
    marker.draw();
}
//--------------------------------------------------------------
void ofxScreenSelector::mouseMoved(int x, int y ){
    marker.mouseMoved(x,  y );
}
//--------------------------------------------------------------
void ofxScreenSelector::mouseDragged(int x, int y, int button){
    marker.mouseDragged( x, y, button);
}
//--------------------------------------------------------------
void ofxScreenSelector::mousePressed(int x, int y, int button){
    marker.mousePressed( x,  y,  button);
}
//--------------------------------------------------------------
void ofxScreenSelector::mouseReleased(int x, int y, int button){
    marker.mouseReleased( x,  y,  button);
    if(marker.isBeenResized() ){
        selectionFormBackgoundImage->clear();
        selectionFormBackgoundImage->allocate( retinalFactor * marker.getSize().x, retinalFactor *  marker.getSize().y, OF_IMAGE_COLOR_ALPHA);
    }
}
//--------------------------------------------------------------
void ofxScreenSelector::windowResized(int w, int h){
    marker.windowResized( w,  h);
}
//--------------------------------------------------------------
unsigned char* ofxScreenSelector::getPixels(){
    return selectionFormBackgoundImage->getPixels();
}
//--------------------------------------------------------------
ofPoint ofxScreenSelector::getSize(){
    return ofPoint( selectionFormBackgoundImage->width , selectionFormBackgoundImage->height );
}
//--------------------------------------------------------------
ofImage* ofxScreenSelector::getImage(){
    return selectionFormBackgoundImage;
}





