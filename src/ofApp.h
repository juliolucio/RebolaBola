#pragma once
#include "ofMain.h"
#include "ofxOPC.h"
#include "ofxGui.h"
#include "ofArduino.h"
#include "ofxScreenSelector.h"

class ofApp : public ofBaseApp{
    
public:
    
    typedef enum {  CAPTURE_FROM_IMAGE_FILE,
                    CAPTURE_FROM_SCREEN,
                    CAPTURE_FROM_VIDEO_FILE,
                    CAPTURE_FROM_CAMERA } captuteTypes;
    
    void setup();
    void openChildApp();
    void update();
    void drawOutput( int x , int y, int width , int height );
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOPC opcClient;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    
    ofImage  imageFromFile;
    ofVideoPlayer video;
    ofVideoGrabber camera;
    
    ofxScreenSelector selector;
    bool retina;
    int rx;
    
    ofImage screenBackground;
    ofImage inputImageResult;
    
    ofImage	output;
    
    int numPixels;
    
    int captureWidth;
    int captureHeight;
    
    int outputWidth;
    int outputHeight;
    
    int updateIndex;
    captuteTypes captureType;
    
    int micrisecondsBetwenLineUpdate;
    
    int numPixelsCapturePerOutputWidth;
    int numPixelsCapturePerOutputHeight;
    
    unsigned char* pixelsOutput;
    
    void setCaptureType( captuteTypes type  , string fileName );
    void setPixelLineRate( int & lineRate );
    
    void resetSizes();
    void updateImageInput();
    void updateImageBackground();
    void updateImageOutput();
    void sendFrameToLedsDivided();
    void sendFrameToLedsSingle();
    
    int lastTimeSend;
    
    ofArduino arduino;

    unsigned long long lastTimerUserInteracted;

    ofxIntSlider linesPerSecond;
    ofxFloatSlider pixelAtenuation;
    ofxToggle testX;
    ofxToggle testY;
    ofxToggle testXY;
    ofxToggle isGammaCorrecting;
    ofxToggle isAtenuated;
    ofxToggle updatingStripe01;
    ofxToggle updatingStripe02;
    ofxToggle updatingAlternating;
    

    
    ofxPanel gui;
    
};
