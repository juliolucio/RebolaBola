#pragma once
#include "ofMain.h"
class ofxNeoPixelLedArc;
class ofxNeoPixelLedSphere {
    
public:
    ofxNeoPixelLedSphere();
    void setup( int theNumArcs , int theNumLedsPerArc );
    void update();

    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    std::vector<ofxNeoPixelLedArc*> arcs;
};
