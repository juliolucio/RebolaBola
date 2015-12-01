#pragma once
#include "ofMain.h"

class ofxNeoPixelLedArc;
class ofxProjectableImage;

class ofxNeoPixelLedSphere {
public:
    ofxNeoPixelLedSphere();
    ~ofxNeoPixelLedSphere();
    void setup( int theNumArcs , int theNumLedsPerArc );
    void update( float sphereAngleTeta );
    
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void clear();
    
    std::vector<ofxNeoPixelLedArc*> arcs;
    float deltaAngleArcToArc;
};
