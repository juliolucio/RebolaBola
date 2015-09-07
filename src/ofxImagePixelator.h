#pragma once
#include "ofMain.h"

class ofxImagePixelator {
    
public:
    ofxImagePixelator();
    void setImage( ofImage* theInputImage ,int theOutputHeight , bool theIsRetinal );
    void update();
    void draw();
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    ofPoint getCapureSize();
    unsigned char* getOutputPixels();
    ofImage* getOutputImage();

private:
    float proportions;
    ofPoint numPixelsInputPerPixelsOutput;
    ofPoint pixelDensity;
    ofImage* inputImage;
    ofImage* outputImage;
    
    ofPoint imageInputSizePrevious;
    
};
