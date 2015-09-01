#pragma once
#include "ofMain.h"

class ofxNeoPixelLed;
class ofxProjectableImage;
class ofxNeoPixelLedArc {
    
public:
    ofxNeoPixelLedArc();
    void setup(  int theNumLeds , float theRadius );
    void update( float theAngleTeta );
    void projectImage( ofxProjectableImage* theProjectableImage );
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    float angleTeta;
    std::vector<ofxNeoPixelLed*> leds;
    float radius;
    ofxProjectableImage* projectableImage;
 };
