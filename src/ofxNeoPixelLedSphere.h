#pragma once
#include "ofMain.h"

class ofxNeoPixelLedArc;
class ofxProjectableImage;

class ofxNeoPixelLedSphere {
public:
    ofxNeoPixelLedSphere();
    void setup( int theNumArcs , int theNumLedsPerArc , float theRadius );
    void update( float sphereAngleTeta );

    void projectImage( ofxProjectableImage* theProjectableImage );
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    std::vector<ofxNeoPixelLedArc*> arcs;
    float deltaAngleArcToArc;
    float radius;
    
    ofxProjectableImage* projectableImage;
};
