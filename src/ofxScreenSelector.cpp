#include "ofxScreenSelector.h"
//--------------------------------------------------------------
ofxScreenSelector::ofxScreenSelector(){
    setup();
}
//--------------------------------------------------------------
void ofxScreenSelector::update( ofPoint theWindowPosition ){
    ofPoint position = theWindowPosition;
}
//--------------------------------------------------------------
void ofxScreenSelector::setup(){
    position = ofPoint( 100 , 100 );
    selectionSize = ofPoint( 100 , 100 );
    markerRadius = 6;
    isVisible = true;
    isDragingMarkerPosition = false;
    isDragingMarkerSize = false;
    isJustRelese = false;
}
//--------------------------------------------------------------
void ofxScreenSelector::draw(){
    ofSetColor( 100 , 255 , 100 );
    ofNoFill();
    ofRect( position.x , position.y , selectionSize.x , selectionSize.y);
    ofFill();
    ofCircle( position , markerRadius );
    ofCircle( position + selectionSize , markerRadius );
    ofSetColor(255);
}
//--------------------------------------------------------------
void ofxScreenSelector::mouseMoved(int x, int y ){
    if( isDragingMarkerPosition ){
        position = ofPoint( x , y );
        if( ( position + selectionSize ).x > ofGetWidth() )
            selectionSize.x -= ( position + selectionSize ).x - ofGetWidth();
        if( ( position + selectionSize ).y > ofGetHeight() )
            selectionSize.y -= ( position + selectionSize ).y - ofGetHeight();
    }
    if( isDragingMarkerSize )
        if( x > position.x + markerRadius && y > position.y + markerRadius )
            selectionSize = ofPoint( x , y ) - position;
}
//--------------------------------------------------------------
void ofxScreenSelector::mouseDragged(int x, int y, int button){
    if( isDragingMarkerPosition ){
        position = ofPoint( x , y );
        if( ( position + selectionSize ).x > ofGetWidth() )
            selectionSize.x -= ( position + selectionSize ).x - ofGetWidth();
        if( ( position + selectionSize ).y > ofGetHeight() )
            selectionSize.y -= ( position + selectionSize ).y - ofGetHeight();
    }
    if( isDragingMarkerSize )
        if( x > position.x + markerRadius && y > position.y + markerRadius )
            selectionSize = ofPoint( x , y ) - position;
}
//--------------------------------------------------------------
void ofxScreenSelector::mousePressed(int x, int y, int button){
    if( ( ofPoint( x , y ) - position ).length() < markerRadius / 2  && button == 0 )
        isDragingMarkerPosition = true;
    if( ( ofPoint( x , y ) - (position + selectionSize ) ).length() < markerRadius / 2  && button == 0 )
        isDragingMarkerSize = true;
    isJustRelese = false;
}
//--------------------------------------------------------------
void ofxScreenSelector::mouseReleased(int x, int y, int button){
    if( isDragingMarkerPosition ){
        isDragingMarkerPosition = false;
        isJustRelese = true;
    }
    if( isDragingMarkerSize ){
        isDragingMarkerSize = false;
        isJustRelese = true;
    }
}
//--------------------------------------------------------------
void ofxScreenSelector::windowResized(int w, int h){
}
