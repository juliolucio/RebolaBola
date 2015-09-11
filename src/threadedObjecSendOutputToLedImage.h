#pragma once


#include "ofThread.h"
#include "ofxOPC.h"
#include "threadedObjecRecieveSensorReadings.H"

class ThreadedObjecSendOutputToLedImage: public ofThread{
private:
    ofxOPC opcClient;
    unsigned char* pixelsOutput;
    
    int indexStripe01;
    int indexStripe02;
    
    double angle;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    //--------------------------------------------------------------
    void sendStripeForAngleTo01(){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip01.clear();
            int colum;
            if( angle <= PI )
                colum = ofMap( angle , 0 , PI , 0 , outputImage->width-1  );
            else
                colum = ofMap( angle , PI , 2.0 * PI ,  outputImage->width-1 , 0 );
            
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
    //--------------------------------------------------------------
    void sendStripeForAngleTo02(){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip02.clear();
            int colum;
            double angle2 = angle + PI;
            if( angle2 >= 2 * PI )
                angle2 = fmod( angle2 , 2.0 * PI );
            if( angle2 <= PI )
                colum = ofMap( angle2 , 0 , PI , 0 , outputImage->width-1  );
            else
                colum = ofMap( angle2 , PI , 2.0 * PI ,  outputImage->width-1 , 0 );

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
    //--------------------------------------------------------------

    void calculateAngleFormEulerAngles(){
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

        float heading   = sensor->getValue01();
        float attitude  = sensor->getValue02();
        float bank      = sensor->getValue03();
        
        float c1 = cosf( heading / 2.0 );
        float c2 = cosf( attitude / 2.0 );
        float c3 = cosf( bank / 2.0 );
        float s1 = sinf( heading / 2.0 );
        float s2 = sinf( attitude / 2.0 );
        float s3 = sinf( bank / 2.0);
        
        angle = 2 * acosf( c1 * c2 * c3 - s1 * s2 * s3 );
        cout << "Angle = " << angle << "\n";
        if( angle >= 2 * PI )
            angle = fmod ( angle , 2.0 * PI);
        
    }

    
    
protected:
    ofImage* outputImage;
    ThreadedObjecRecieveSensorReadings* sensor;
    bool isSendingStripe01;
    bool isSendingStripe02;
    //--------------------------------------------------------------
public:
    ThreadedObjecSendOutputToLedImage(){
        opcClient.setup("127.0.0.1", 7890);
        if( !opcClient.isConnected() )
            cout << " not conected to FadeCandy\n";
        
        indexStripe01 = 1;
        indexStripe02 = 8;
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
                calculateAngleFormEulerAngles();
                if( isSendingStripe01 )
                    sendStripeForAngleTo01( );
                if( isSendingStripe02 )
                    sendStripeForAngleTo02(  );
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
    void setDrawingStripe01( bool isDrawing ){
        isSendingStripe01 = isDrawing;
    }
    //--------------------------------------------------------------
    void setDrawingStripe02( bool isDrawing ){
        isSendingStripe02 = isDrawing;
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
