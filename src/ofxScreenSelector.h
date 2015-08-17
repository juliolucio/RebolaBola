#pragma once
#include "ofMain.h"

class ofxScreenSelector {
    
public:
    ofxScreenSelector();
    void setup();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    ofPoint position;
    ofPoint selectionSize;
    int markerRadius;
    bool isVisible;
    bool isDragingMarkerPosition;
    bool isDragingMarkerSize;
};
