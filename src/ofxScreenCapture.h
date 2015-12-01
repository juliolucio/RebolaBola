#pragma once
#include "ofMain.h"

class ofxScreenCapture {
public:
    ofxScreenCapture();
    void setup(bool isRetina);
    void update();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
private:
    ofImage backgroundImage;
    bool retina;
};
