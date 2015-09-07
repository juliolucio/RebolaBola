#pragma once
#include "ofMain.h"
#include "ofxNeoPixelLedSphere.h"
#include "ofxNeoPixelLedArc.h"
#include "ofxNeoPixelLed.h"

class ofxProjectableImage {

public:
    ofxProjectableImage();
    void setup();
    void update( float theAngleTeta );
    void draw(int x , int y);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void setImageInput( ofImage* &theInputImage );
    int getNumArcs();
    int getNumPixelsPerArc();
    ofColor getSpherePixelColorFromIndexAndArc( int indexArc , int indexPixel );
    
private:
    void updateAngleTeta(float theAngleTeta);
    void updateSphereColor();
    
    ofImage* imageInput;
    
    float spatialWidth;
    float spatialHeight;
    
    float imageDistance;
    
    ofxNeoPixelLedSphere sphere;
};
