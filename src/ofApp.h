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
    
    void setupGuiInput();
    void setupGuiDebug();
    void setupGuiOutput();
    void openChildApp();
    void update();
    void drawOutput( int x , int y, int width , int height );
    void drawInputResul( int x , int y, int width , int height );

    void drawLastColum( int x , int y , int width , int stripeIndex );
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
    
    ofImage  imageFromFile;
    ofVideoPlayer video;
    ofVideoGrabber camera;
    
    ofxScreenSelector selector;
    bool retina;
    int rx;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    
    int indexStripe0;
    int indexStripe1;
    
    ofImage screenBackground;
    ofImage inputImageResult;
    
    ofImage	output;
    
    int captureWidth;
    int captureHeight;
    
    int outputWidth;
    int outputHeight;
    
    int lastColunSnedStripe01;
    int lastColunSnedStripe02;
    
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
    void sendFrameToLedStripe01();
    void sendFrameToLedStripe02();
    void tryToSendColumnToStripe01();
    
    long long lastTimeSendtripe01;
    long long lastTimeSendtripe02;
    long long currenTime;
    
    ofArduino arduino;

    unsigned long long lastTimerUserInteracted;

    //panel input
    ofxPanel guiInput;
    
    //panel debug
    ofxToggle isDebuging;
    ofxToggle isDebugTestX;
    ofxToggle isDebugTestY;
    ofxToggle isDebugisDebugTestXY;
    ofxPanel guiDebug;
    
    //panel output
    ofxToggle isAtenuated;
    ofxFloatSlider pixelAtenuation;
    ofxIntSlider linesPerSecond;
    ofxToggle isGammaCorrecting;
    ofxToggle isUpdatingStripe01;
    ofxToggle isUpdatingStripe02;
    ofxPanel guiOutput;
};
