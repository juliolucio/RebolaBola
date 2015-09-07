#pragma once
#include "ofMain.h"

class ofxProjectableImage;

class ofxNeoPixelLed {
public:
    ofxNeoPixelLed();
    void setup( float theAngleFi );
    void update( float theAngleTeta );
    
    
    float angleTeta;
    float angleFi;
    ofColor currentColor;
    std::vector<ofColor> colorsArray;
 };
