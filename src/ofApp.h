#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofArduino.h"
#include "ofxScreenSelector.h"
#include "threadedObjecSendOutputToLedStripest.h"

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

    void drawLastColum( int x , int y , int stripeIndex );
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
    
    //ofxOPC opcClient;
    
    ofImage  imageFromFile;
    ofVideoPlayer video;
    ofVideoGrabber camera;
    
    ofxScreenSelector selector;
    bool retina;
    int rx;
    
    vector<ofColor> colorsStrip02;
    

    
    ofImage screenBackground;
    ofImage inputImageResult;
    
    ofImage	output;
    
    int captureWidth;
    int captureHeight;
    
    int outputWidth;
    int outputHeight;
    
    int updateIndex;
    captuteTypes captureType;
    
    int microsecondsBetwenLineUpdate;
    
    int numPixelsCapturePerOutputWidth;
    int numPixelsCapturePerOutputHeight;
    
    unsigned char* pixelsOutput;
    
    void setCaptureType( captuteTypes type  , string fileName );
    
    void changedColumDrawingDelay( int & lineRate );
    void setInputModeImage();
    void setInputModeVideo();
    void setInputModeCamera();
    void setInputModeScreen();
    
    void processOpenFileImage( ofFileDialogResult openFileResult );
    void processOpenFileVideo( ofFileDialogResult openFileResult );
    
    void resetSizes();
    void updateImageInput();
    void updateImageBackground();
    void updateImageOutput();
    
    unsigned long long lastUpdateTime;

    
    ofArduino arduino;

    unsigned long long lastTimerUserInteracted;

    //panel input
    ofxPanel guiInput;
    ofxButton butonImage;
    ofxButton butonVideo;
    ofxButton butonCamera;
    ofxButton butonScreen;
    
    //panel debug
    ofxToggle isDebuging;
    ofxToggle isDebugTestX;
    ofxToggle isDebugTestY;
    ofxToggle isDebugisDebugTestXY;
    ofxPanel guiDebug;
    
    //panel output
    ofxToggle isAtenuated;
    ofxFloatSlider pixelAtenuation;
    ofxIntSlider columDrawingDelay;
    ofxToggle isGammaCorrecting;
    ofxToggle isUpdatingStripe01;
    ofxToggle isUpdatingStripe02;
    ofxPanel guiOutput;
    
    ThreadedObjecSendOutputToLedStripest threadedObjecSendOutputToLedStripest;
};
