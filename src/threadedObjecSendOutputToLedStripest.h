#pragma once


#include "ofThread.h"
#include "ofxOPC.h"

class ThreadedObjecSendOutputToLedStripest: public ofThread{
private:
    ofxOPC opcClient;
    unsigned long long lastUpdateTime;
    unsigned char* pixelsOutput;
    
    int indexStripe0;
    int indexStripe1;
    
    int lastColunSnedStripe01;
    unsigned long long lastTimeSendtripe01;

    void tryToSendColumnToStripe01(){
        unsigned long long delta = ofGetElapsedTimeMicros() - lastTimeSendtripe01;
        if( opcClient.isConnected() && pixelsOutput && delta > delayBetwenLines ){
            cout << " delta thread= " << delta << "\n";
            vector<ofColor> colorsStrip01;
            for( int pixel = 0 ; pixel < outputImage.height ; pixel ++ ){
                int pixelIndex =  outputImage.width * pixel + lastColunSnedStripe01;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip01.push_back( pixelColor );
            }
            opcClient.writeChannel( indexStripe0 , colorsStrip01 );
            lastColunSnedStripe01++;
            if( lastColunSnedStripe01 >= outputImage.width )
                lastColunSnedStripe01 = 0;
             lastTimeSendtripe01 = ofGetElapsedTimeMicros();
        }
    }

    
protected:
    ofImage outputImage;
    unsigned long long delayBetwenLines;
    
public:
    ThreadedObjecSendOutputToLedStripest(){
        opcClient.setup("127.0.0.1", 7890);
        lastUpdateTime = 0;
        lastColunSnedStripe01 = 0;
        lastTimeSendtripe01 = 0;
        indexStripe0 = 1;
        indexStripe1 = 8;
        delayBetwenLines = 10;
    }
    
    void setOutputSize( ofPoint outputSize ){
        ofScopedLock lock(mutex);
        outputImage.clear();
        outputImage.allocate( outputSize.x , outputSize.y , OF_IMAGE_COLOR );
    }

    void start(){
        startThread();
    }
    
    void stop(){
        stopThread();
    }
    
    void threadedFunction(){
        while( isThreadRunning() ){
            if(lock()){
                //unsigned long long delta = ofGetElapsedTimeMicros() - lastUpdateTime;
                //cout << " delta Thread = " << delta << "\n";
                tryToSendColumnToStripe01();
                lastUpdateTime = ofGetElapsedTimeMicros();
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void setOutputImage( ofImage* theOutputImage){
        if(lock()){
            if( theOutputImage )
                outputImage.setFromPixels((theOutputImage->getPixels() ), theOutputImage->width , theOutputImage->height, OF_IMAGE_COLOR );
            pixelsOutput = outputImage.getPixels();
            unlock();
        }
        else{
            cout << "setOutputImage()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void setOutputColumDelay( int theDelay ){
        ofScopedLock lock(mutex);
        delayBetwenLines = theDelay;
    }
};
