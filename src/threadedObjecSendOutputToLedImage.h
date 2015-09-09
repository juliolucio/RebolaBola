#pragma once


#include "ofThread.h"
#include "ofxOPC.h"
#include "threadedObjecRecieveSensorReadings.H"

class ThreadedObjecSendOutputToLedImage: public ofThread{
private:
    ofxOPC opcClient;
    unsigned long long lastUpdateTime;
    unsigned char* pixelsOutput;
    
    int indexStripe01;
    int indexStripe02;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    
    int lastColunSnedStripe01;
    unsigned long long lastTimeSendtripe01;
    
    int lastColunSnedStripe02;
    unsigned long long lastTimeSendtripe02;
    //--------------------------------------------------------------
    
    void sendStripeForAngleTo01( double theAngle ){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip01.clear();
            int colum = ofMap( theAngle , 0.0f , -180 , 180 , outputImage->width );
            for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                int pixelIndex =  outputImage->width * pixel + colum;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,
                                             pixelsOutput[ 3 * pixelIndex + 1 ] ,
                                             pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip01.push_back( pixelColor );
            }
            if( opcClient.isConnected() )
                opcClient.writeChannel( indexStripe01 , colorsStrip01 );
        }
    }
    
    
//    angle = 2 * acos(c1c2c3 - s1s2s3)
//    x = s1 s2 c3 +c1 c2 s3
//    y = s1 c2 c3 + c1 s2 s3
//    z = c1 s2 c3 - s1 c2 s3
//    
//    to normalise divide x,y and z by:
//    
//    sqrt(x2 + y2 + z2) = sqrt((s1 s2 c3 +c1 c2 s3)2+(s1 c2 c3 + c1 s2 s3)2+(c1 s2 c3 - s1 c2 s3)2)
//    
//where:
//    
//    c1 = cos(heading / 2)
//    c2 = cos(attitude / 2)
//    c3 = cos(bank / 2)
//    s1 = sin(heading / 2)
//    s2 = sin(attitude / 2)
//    s3 = sin(bank / 2)
    
    
    void sendStripeForAngleTo02( double theAngle ){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip02.clear();
            int colum = ofMap( theAngle , -90.0f , 90.0f , 0 , outputImage->width );
            for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                int pixelIndex =  outputImage->width * pixel + colum;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,
                                             pixelsOutput[ 3 * pixelIndex + 1 ] ,
                                             pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip02.push_back( pixelColor );
            }
            if( opcClient.isConnected() )
                opcClient.writeChannel( indexStripe02 , colorsStrip02 );
        }
    }

    
    void tryToSendColumnToStripe01(){
        if( !opcClient.isConnected() )
            return;
        //cout << " try thread \n";
        unsigned long long delta = ofGetElapsedTimeMicros() - lastTimeSendtripe01;
        if( delta > delayBetwenLines ){
            cout << " delta thread= " << delta << "\n";
            if( pixelsOutput ){
                colorsStrip01.clear();
                for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                    int pixelIndex =  outputImage->width * pixel + lastColunSnedStripe01;
                    ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
                    colorsStrip01.push_back( pixelColor );
                }
                
                if( opcClient.isConnected() )
                    opcClient.writeChannel( indexStripe01 , colorsStrip01 );
                
                lastColunSnedStripe01++;
                if( lastColunSnedStripe01 >= outputImage->width )
                    lastColunSnedStripe01 = 0;
                lastTimeSendtripe01 = ofGetElapsedTimeMicros();
            }
        }
    }
    
    void tryToSendColumnToStripe02(){
        if( !opcClient.isConnected() )
            return;
        //cout << " try thread \n";
        unsigned long long delta = ofGetElapsedTimeMicros() - lastTimeSendtripe01;
        //cout << " delta thread= " << delta << "\n";
        if( delta > delayBetwenLines ){
            pixelsOutput = outputImage->getPixels();
            if( pixelsOutput ){
                colorsStrip02.clear();
                for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                    int pixelIndex =  outputImage->width * pixel + lastColunSnedStripe02;
                    ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
                    colorsStrip02.push_back( pixelColor );
                }
                if( opcClient.isConnected() )
                    opcClient.writeChannel( indexStripe02 , colorsStrip02 );
                
                lastColunSnedStripe02++;
                if( lastColunSnedStripe02 >= outputImage->width )
                    lastColunSnedStripe02 = 0;
                lastTimeSendtripe02 = ofGetElapsedTimeMicros();
            }
        }
    }
    //--------------------------------------------------------------
    
protected:
    ofImage* outputImage;
    ThreadedObjecRecieveSensorReadings* sensor;
    unsigned long long delayBetwenLines;
    //--------------------------------------------------------------
public:
    ThreadedObjecSendOutputToLedImage(){
        opcClient.setup("127.0.0.1", 7890);
        if( !opcClient.isConnected() )
            cout << " not conected to FadeCandy\n";
        lastUpdateTime = 0;
        
        lastColunSnedStripe01 = 0;
        lastTimeSendtripe01 = 0;
        
        lastColunSnedStripe02 = 2;
        lastTimeSendtripe02 = 2;
        
        indexStripe01 = 1;
        indexStripe02 = 8;
        
        delayBetwenLines = 10;
    }
    //--------------------------------------------------------------
    void start(){
        startThread();
    }
    //--------------------------------------------------------------
    void stop(){
        stopThread();
    }
    //--------------------------------------------------------------
    void threadedFunction(){
        while( isThreadRunning() ){
            if(lock()){
                pixelsOutput = outputImage->getPixels();
                cout << " angle = " << sensor->getAngleBetta() << "\n";
                sendStripeForAngleTo01( sensor->getAngleBetta() );
                //sendStripeForAngleTo02( sensor->getAngleBetta() + 180 );
                //tryToSendColumnToStripe01();
                //tryToSendColumnToStripe02();
                //lastUpdateTime = ofGetElapsedTimeMicros();
                //sleep(1);
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    //--------------------------------------------------------------
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
    //--------------------------------------------------------------
    void setSensorThread( ThreadedObjecRecieveSensorReadings*  theSensor ){
        if(lock()){
            if( theSensor ){
                sensor = theSensor;
                cout << " Sensor Seted \n";
            }
            unlock();
        }
        else{
            cout << "setOutputImage()" << "Unable to lock mutex." << "\n";
        }
    }
    
    //--------------------------------------------------------------
    void setLinesPerSecond( double theLinesPerSecond ){
        if(lock()){
            delayBetwenLines = 1000000.0f / theLinesPerSecond;
            cout << " delta line = " << delayBetwenLines << "\n";
            unlock();
        }
        else{
            cout << "setOutputImage()" << "Unable to lock mutex." << "\n";
        }
    }
    //--------------------------------------------------------------
    void draw( int x , int y ){
        ofScopedLock lock(mutex);
        outputImage->draw( x , y );
    }
    
    //--------------------------------------------------------------
    bool isConectedToFadecandy( ){
        ofScopedLock lock(mutex);
        if( opcClient.isConnected() )
            return true;
        return false;
    }
    //--------------------------------------------------------------
    void drawLastColum( int x , int y ,  int stripeIndex ){
        ofScopedLock lock(mutex);
        if(!colorsStrip01.size())
            return;
        //cout << " Draw column \n";
        int width = ofGetHeight() / 32;
        for( int i = 0 ; i < colorsStrip01.size() ; i ++ ){
            ofFill();
            ofSetHexColor( 0x229922 );
            ofRect( x , y + width * i , width , width  );
            if( !(i % 10) )
                ofDrawBitmapString(ofToString(i), ofPoint( x + width ,y + width * i) );
            if( stripeIndex == 0 )
                ofSetColor( colorsStrip01[i]);
            else
                ofSetColor( colorsStrip02[i]);
            ofRect( x+2 , y + width * i + 2 , width - 2 , width - 2  );
            ofSetColor(255);
        }
    }
};
