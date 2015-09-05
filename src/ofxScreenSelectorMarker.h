#pragma once
#include "ofMain.h"

class ofxScreenSelectorMarker {
    
public:
    ofxScreenSelectorMarker();
    void setup();
    void update();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    bool isBeenResized();
    ofPoint getPosition();
    void setPosition( ofPoint thePosition );
    ofPoint getSize();
    void setSize( ofPoint theSize );

private:
    ofPoint position;
    ofPoint selectionSize;
    int markerRadius;
    bool isVisible;
    bool isDragingMarkerPosition;
    bool isDragingMarkerSize;
    bool isJustRelese;
    bool isJustResized;
};
