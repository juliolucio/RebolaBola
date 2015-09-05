#pragma once
#include "ofMain.h"

#include "ofxScreenSelectorMarker.h"

class ofxScreenSelector {
public:
    ofxScreenSelector();
    void setup( bool theRetina , ofPoint thePosition , ofPoint theSize );
    void update();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    unsigned char* getPixels();
    ofPoint getSize();
    ofImage* getImage();
    
private:
    ofxScreenSelectorMarker marker;
    ofImage* selectionFormBackgoundImage;
    bool isRetinaScreen;
    int retinalFactor;

};
