#pragma once
#include "ofMain.h"
#include "ofxGui.h"

#include "ofxScreenSelector.h"
#include "ofxScreenCapture.h"
#include "ofxImagePixelator.h"

#include "threadedObjecSendOutputToLedStripest.h"
#include "threadedObjecRecieveSensorReadings.h"

#include "ofxNeoPixelLedSphere.h"
#include "ofxNeoPixelLedArc.h"
#include "ofxNeoPixelLed.h"
#include "ofxProjectableImage.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
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
    
private:
    
    //properties
    typedef enum {  CAPTURE_FROM_IMAGE_FILE,
        CAPTURE_FROM_SCREEN,
        CAPTURE_FROM_VIDEO_FILE,
        CAPTURE_FROM_CAMERA } captuteTypes;
    captuteTypes captureType;
    
    bool isRetina;
    int numPixels;
    
    ofxScreenCapture backgroudCapture;
    
    ofImage* imageInput;
    ofImage* imageOutput;
    
    ofVideoPlayer video;
    ofVideoGrabber camera;
    
    ofxScreenSelector screenSelectionCapture;
    
    ofxImagePixelator* pixelator;
    
    int microsecondsBetwenLineUpdate;
    
    ThreadedObjecSendOutputToLedStripest threadLedSender;
    threadedObjecRecieveSensorReadings threadSensorReciver;

    ofxProjectableImage* projectableImage;
    
    unsigned long long lastTimerUserInteracted;
    
    //gui
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
    
    void setCaptureType( captuteTypes type  , string fileName );
    void setProjection( ofVec3f theImagePosition , ofVec3f theImageNormal );
    void updateTests();
    
    //drawing
    void drawOutput( int x , int y, int width , int height );
    void drawInputResul( int x , int y, int width , int height );
    void drawLastColum( int x , int y , int stripeIndex );
    
    //gui
    void setupGuiInput();
    void setupGuiDebug();
    void setupGuiOutput();
    
    void changedColumDrawingDelay( int & lineRate );
    
    void setInputModeImage();
    void setInputModeVideo();
    void setInputModeCamera();
    void setInputModeScreen();
    
    void processOpenFileImage( ofFileDialogResult openFileResult );
    void processOpenFileVideo( ofFileDialogResult openFileResult );

   
    
    
};
