#include "ofxScreenSelectorMarker.h"
//privates
//--------------------------------------------------------------
ofxScreenSelectorMarker::ofxScreenSelectorMarker(){
    setup();
}

//--------------------------------------------------------------
void ofxScreenSelectorMarker::setup(){
    position = ofPoint( 0 , 0 );
    selectionSize = ofPoint( 64 , 64 );
    markerRadius = 6;
    isVisible = true;
    isDragingMarkerPosition = false;
    isDragingMarkerSize = false;
    isJustRelese = false;
}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::update(){
}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::draw(){
    ofNoFill();
    ofSetColor( 100 , 255 , 100 );
    ofRect( position.x , position.y , selectionSize.x , selectionSize.y);
    
    //moving
    ofFill();
    ofSetColor( 200 , 50 , 50 );
    ofCircle( position , markerRadius + 2 );
    ofSetColor( 50 , 200 , 50 );
    ofCircle( position , markerRadius );
    ofSetColor( 50 , 50 , 200 );
    ofCircle( position , markerRadius  - 2);
    
    //scaling
    ofSetColor( 200 , 50 , 50 );
    ofCircle( position + selectionSize , markerRadius  + 2);
    ofSetColor( 50 , 200 , 50 );
    ofCircle( position + selectionSize , markerRadius );
    ofSetColor( 50 , 50 , 200 );
    ofCircle( position + selectionSize , markerRadius - 2);
    ofSetColor(255);
    
    //corners
    ofSetColor( 100 , 255 , 100 );
    ofCircle( position.x + selectionSize.x , position.y , markerRadius  - 2);
    ofCircle( position.x , position.y + selectionSize.y , markerRadius  - 2);
    
    ofSetColor(255);

}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::mouseMoved(int x, int y ){
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
void ofxScreenSelectorMarker::mouseDragged(int x, int y, int button){
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
void ofxScreenSelectorMarker::mousePressed(int x, int y, int button){
    if( ( ofPoint( x , y ) - position ).length() < markerRadius / 2  && button == 0 )
        isDragingMarkerPosition = true;
    if( ( ofPoint( x , y ) - (position + selectionSize ) ).length() < markerRadius / 2  && button == 0 )
        isDragingMarkerSize = true;
    isJustRelese = false;
}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::mouseReleased(int x, int y, int button){
    if( isDragingMarkerPosition ){
        isDragingMarkerPosition = false;
        isJustRelese = true;
    }
    if( isDragingMarkerSize ){
        isDragingMarkerSize = false;
        isJustRelese = true;
        isJustResized = false;
    }
}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::windowResized(int w, int h){
    
}

//publics
//--------------------------------------------------------------
bool ofxScreenSelectorMarker::isBeenResized(){
    if( isJustResized ){
        isJustResized = false;
        return true;
    }
    return false;
}

//--------------------------------------------------------------
ofPoint ofxScreenSelectorMarker::getPosition(){
    return position;
}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::setPosition( ofPoint thePosition ){
    position = thePosition;
}
//--------------------------------------------------------------
ofPoint ofxScreenSelectorMarker::getSize(){
    return selectionSize;
}
//--------------------------------------------------------------
void ofxScreenSelectorMarker::setSize( ofPoint theSize ){
    selectionSize = theSize;
}

