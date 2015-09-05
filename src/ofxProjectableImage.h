#pragma once
#include "ofMain.h"
#include "ofxNeoPixelLedSphere.h"
#include "ofxNeoPixelLedArc.h"
#include "ofxNeoPixelLed.h"

class ofxProjectableImage {

public:
    ofxProjectableImage();
    void setup();
    void setImage( ofImage* &theInputImage );
    void updateAngleTeta(float theAngleTeta);
    void draw(int x , int y);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void updateSphereColor();
    ofImage* getImage();
    
    ofImage* image;
    float spatialWidth;
    float spatialHeight;
    
    float imageDistance;
    
    ofxNeoPixelLedSphere* sphere;
};
