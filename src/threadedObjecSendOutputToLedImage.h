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
    
    double angle01;
    double angle02;
    
    unsigned long long time;
    
    int colum01;
    int colum02;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    
    unsigned long long lastUpdateTime;
    
protected:
    ofImage* outputImage;
    ThreadedObjecRecieveSensorReadings* sensor;
    bool isSendingStripe01;
    bool isSendingStripe02;
    float phaseShift;
    
public:
    ThreadedObjecSendOutputToLedImage(){
        opcClient.setup("127.0.0.1", 7890);
        if( !opcClient.isConnected() )
            cout << " not connected to FadeCandy\n";
        
        indexStripe01 = 1;
        indexStripe02 = 8;
        cout << "Connected to FadeCandy\n";
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
                updateColumnIndex();
                //cout << "Angle = " << angle << "\n";
                //sendTestAngle();
                if( isSendingStripe01 )
                    sendStripeForAngleTo01();
                if( isSendingStripe02 )
                    sendStripeForAngleTo02();
                lastUpdateTime = ofGetElapsedTimeMicros();
                
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
    void setDelay( float value ){
        ofScopedLock lock(mutex);
        phaseShift = value;
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
    void drawAngle( ofPoint position , ofPoint size ){
        ofScopedLock lock(mutex);
        ofPushMatrix();
        ofTranslate( position.x , position.y );
        
        ofFill();
        ofSetColor( 90 , 90 , 120 );
        ofRect(0 , 0 , size.x , size.y);
        
        ofNoFill();
        ofSetColor( 255 , 255 , 255 );
        ofRect(0 , 0 , size.x , size.y);
        
        ofSetColor( 230 , 230 , 230 );
        ofCircle( size.x/2 , size.y/2 , size.x/4 );
        
        
        ofSetColor( 255 , 0 , 0 );
        ofLine( size.x/2 , size.y/2 , size.x/2 + (float(size.x)/3.0f )*cosf( angle01 ) , size.y/2 + (float(size.x)/3.0f )*sinf(  angle01 ));
        ofLine( size.x/2 , size.y/2 , size.x/2 + (float(size.x)/3.0f )*cosf( angle02 ) , size.y/2 + (float(size.x)/3.0f )*sinf(  angle02 ));
        
        ofFill();
        ofSetColor( 120 , 230 , 120 );
        ofCircle( size.x/2 + (float(size.x)/3.0f )*cosf( angle01 ) , size.y/2 + (float(size.x)/3.0f )*sinf( angle01 ) , size.x/16 );
        ofCircle( size.x/2 + (float(size.x)/3.0f )*cosf( angle02 ) , size.y/2 + (float(size.x)/3.0f )*sinf( angle02 ) , size.x/16 );
        
        //deawing motor
        ofPopMatrix();
        ofNoFill();
        ofSetColor( 255 , 255 , 255 );
        
        std::string angleStr = "a1 = ";
        angleStr += ofToString( angle01 , 4 );
        angleStr += "\na2 = ";
        angleStr += ofToString( angle02 , 4 );
        angleStr += "\nt = ";
        angleStr += ofToString( time  );
        ofDrawBitmapString(angleStr, position.x + 5 , position.y + size.y + 12 );
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
    void drawOutput( int x , int y ){
        outputImage->draw(x,y);
        ofSetColor(125,125,125);
        ofNoFill();
        ofRect(  x + colum01 - 1 , y - 1 , 3 , outputImage->getHeight() + 2);
        ofRect(  x + colum02 - 1 , y - 1 , 3 , outputImage->getHeight() + 2);
    }
    //--------------------------------------------------------------
    void drawLastColumStripe01( int x , int y , int widthFactor ){
        ofScopedLock lock(mutex);
        if(!colorsStrip01.size())
            return;
        //cout << " Draw column \n";
        int width = ofGetHeight() / 64;
        for( int i = 0 ; i < colorsStrip01.size() ; i ++ ){
            ofFill();
            ofSetHexColor( 0x229922 );
            ofRect( x , y + width * i , widthFactor * width , width  );
            
            if( !(i % 10) )
                ofDrawBitmapString(ofToString(i), ofPoint( x + width ,y + width * i) );
            ofSetColor( colorsStrip01[ i ] );
            ofRect( x+2 , y + width * i + 2 , widthFactor * width - 2 , width - 2  );
            ofSetColor(255);
        }
    }
    //--------------------------------------------------------------
    void drawLastColumStripe02( int x , int y , int widthFactor ){
        ofScopedLock lock(mutex);
        if(!colorsStrip02.size())
            return;
        //cout << " Draw column \n";
        int width = ofGetHeight() / 64;
        for( int i = 0 ; i < colorsStrip02.size() ; i ++ ){
            ofFill();
            ofSetHexColor( 0x229922 );
            ofRect( x , y + width * i , widthFactor * width , width  );
            
            if( !(i % 10) )
                ofDrawBitmapString(ofToString(i), ofPoint( x + width ,y + width * i) );
            ofSetColor( colorsStrip02[ i ] );
            ofRect( x+2 , y + width * i + 2 , widthFactor * width - 2 , width - 2  );
            ofSetColor(255);
        }
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    void sendTestAngle(){
        if( !opcClient.isConnected() )
            return;
        colorsStrip01.clear();
        ofColor theColor;
        angle01 = sensor->getValue01();
        if( angle01 < PI )
            theColor = ofColor( 125 , 0 , 0 );
        else
            theColor = ofColor( 0 , 0 , 125 );
        for( int p = 0 ; p < outputImage->getHeight() ; p ++ )
            colorsStrip01.push_back( theColor );
        opcClient.writeChannel( indexStripe01 , colorsStrip01 );
    }
    
private:
    //--------------------------------------------------------------
    void updateColumnIndex(){
        angle01 = sensor->getValue01();
        time = ofGetElapsedTimeMillis();
        angle01 = angle01 - float(phaseShift) * float(time)/10000.0;
        
        angle02 = angle01 + PI;
        
        if( angle01 >= 2 * PI )
            angle01 = fmod( angle01 , 2.0 * PI );
        if( angle02 >= 2 * PI )
            angle02 = fmod( angle02 , 2.0 * PI );
        
        
        colum01 = ofMap( angle01 , 0 , 2*PI , 0 , outputImage->getWidth()-1  );
        colum02 = ofMap( angle02 , 0 , 2*PI , 0 , outputImage->getWidth()-1  );
        
        
        //        if( angle01 < PI )
        //            colum01 = ofMap( angle01 , 0 , PI , 0 , outputImage->getWidth()-1  );
        //        else
        //            colum01 = ofMap( angle01 ,  PI , 2 * PI ,outputImage->getWidth()-1 , 0 );
        //
        //        if( angle02 < PI )
        //            colum02 = ofMap( angle02 , 0 , PI , 0 , outputImage->getWidth()-1  );
        //        else
        //            colum02 = ofMap( angle02 ,  PI , 2 * PI ,outputImage->getWidth()-1 , 0 );
    }
    //--------------------------------------------------------------
    void sendStripeForAngleTo01(){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip01.clear();
            for( int pixel = 0 ; pixel < outputImage->getHeight() ; pixel ++ ){
                int pixelIndex =  outputImage->getWidth() * pixel + colum01;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,
                                             pixelsOutput[ 3 * pixelIndex + 1 ] ,
                                             pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip01.push_back( pixelColor );
            }
            opcClient.writeChannel( indexStripe01 , colorsStrip01 );
        }
    }
    //--------------------------------------------------------------
    void sendStripeForAngleTo02(){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip02.clear();
            for( int pixel = 0 ; pixel < outputImage->getHeight() ; pixel ++ ){
                int pixelIndex =  outputImage->getWidth() * pixel + colum02;
                ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,
                                             pixelsOutput[ 3 * pixelIndex + 1 ] ,
                                             pixelsOutput[ 3 * pixelIndex  + 2 ] );
                colorsStrip02.push_back( pixelColor );
            }
            opcClient.writeChannel( indexStripe02 , colorsStrip02 );
        }
    }
    //--------------------------------------------------------------
};
