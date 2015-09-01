#pragma once
#include "ofMain.h"

class ofxProjectableImage {
    
public:
    ofxProjectableImage();
    void setup( ofImage* theImageToProject, ofVec3f thePosition , ofVec3f theNormal  );
    void update();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    ofImage* image;
    ofVec3f position;
    ofVec3f normal;
};
