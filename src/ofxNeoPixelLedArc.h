#pragma once
#include "ofMain.h"

class ofxNeoPixelLed;
class ofxProjectableImage;
class ofxNeoPixelLedArc {
    
public:
    ofxNeoPixelLedArc();
    ~ofxNeoPixelLedArc();
    void setup(  int theNumLeds );
    void update( float theAngleTeta );
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void clear();
    
    float angleTeta;
    std::vector<ofxNeoPixelLed*> leds;
    
};
