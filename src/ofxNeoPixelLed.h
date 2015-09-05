#pragma once
#include "ofMain.h"

class ofxProjectableImage;

class ofxNeoPixelLed {
public:
    ofxNeoPixelLed();
    void setup( float theAngleFi );
    void update( float theAngleTeta );
    void projectImage( ofxProjectableImage* theProjectableImage );
    
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    float angleTeta;
    float angleFi;
    ofColor currentColor;
    std::vector<ofColor> colorsArray;
    ofxProjectableImage* projectableImage;
 };
