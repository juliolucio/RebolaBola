#pragma once
#include "ofMain.h"

class ofxImagePixelator {
    
public:
    ofxImagePixelator();
    void setImage( ofImage* theInputImage ,int theOutputHeight );
    void update();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    unsigned char* getOutputPixels();
    ofImage* getOutputImage();
    
private:
    float proportions;
    ofPoint pixelDensity;
    ofImage* inputImage;
    ofImage* outputImage;
    
    ofPoint imageInputSizePrevious;
    
    void recalculateOutpuSize( int theOutputHeight );
    void updateOutputImage();
    
};
