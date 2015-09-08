#pragma once


#include "ofThread.h"
#include "ofxOPC.h"

class ThreadedObjecSendOutputToLedImage: public ofThread{
private:
    ofxOPC opcClient;
    unsigned long long lastUpdateTime;
    unsigned char* pixelsOutput;
    
    int indexStripe0;
    int indexStripe1;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    
    int lastColunSnedStripe01;
    unsigned long long lastTimeSendtripe01;
    
    int lastColunSnedStripe02;
    unsigned long long lastTimeSendtripe02;
    
    void tryToSendColumnToStripe01(){
        unsigned long long delta = ofGetElapsedTimeMicros() - lastTimeSendtripe01;
        if( opcClient.isConnected() && pixelsOutput && delta > delayBetwenLines ){
            cout << " delta thread= " << delta << "\n";
            colorsStrip01.clear();
            for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                int pixelIndex =  outputImage->width * pixel + lastColunSnedStripe01;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip01.push_back( pixelColor );
            }
            opcClient.writeChannel( indexStripe0 , colorsStrip01 );
            lastColunSnedStripe01++;
            if( lastColunSnedStripe01 >= outputImage->width )
                lastColunSnedStripe01 = 0;
            lastTimeSendtripe01 = ofGetElapsedTimeMicros();
        }
    }
    
    void tryToSendColumnToStripe02(){
        unsigned long long delta = ofGetElapsedTimeMicros() - lastTimeSendtripe02;
        if( opcClient.isConnected() && pixelsOutput && delta > delayBetwenLines ){
            cout << " delta thread= " << delta << "\n";
            colorsStrip02.clear();
            for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                int pixelIndex =  outputImage->width * pixel + lastColunSnedStripe01;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip02.push_back( pixelColor );
            }
            opcClient.writeChannel( indexStripe1 , colorsStrip02 );
            lastColunSnedStripe02++;
            if( lastColunSnedStripe02 >= outputImage->width )
                lastColunSnedStripe02 = 0;
            lastTimeSendtripe02 = ofGetElapsedTimeMicros();
        }
    }
    
    
    
protected:
    ofImage* outputImage;
    unsigned long long delayBetwenLines;
    
public:
    ThreadedObjecSendOutputToLedImage(){
        opcClient.setup("127.0.0.1", 7890);
        if( !opcClient.isConnected() )
            cout << " not conected to FadeCandy\n";
        lastUpdateTime = 0;
        lastColunSnedStripe01 = 0;
        lastTimeSendtripe01 = 0;
        lastColunSnedStripe01 = 2;
        lastTimeSendtripe01 = 2;
        indexStripe0 = 1;
        indexStripe1 = 8;
        delayBetwenLines = 10;
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
                tryToSendColumnToStripe01();
                tryToSendColumnToStripe02();
                lastUpdateTime = ofGetElapsedTimeMicros();
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void setOutputImage( ofImage* theOutputImage){
        if(lock()){
            if( theOutputImage ){
                outputImage = theOutputImage;
                cout << " Image Seted \n";
            }
            unlock();
        }
        else{
            cout << "setOutputImage()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void draw( int x , int y ){
        ofScopedLock lock(mutex);
        outputImage->draw( x , y );
    }
};
