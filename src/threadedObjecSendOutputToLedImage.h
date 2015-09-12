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
    
    int colum01;
    int colum02;
    
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    
protected:
    ofImage* outputImage;
    ThreadedObjecRecieveSensorReadings* sensor;
    bool isSendingStripe01;
    bool isSendingStripe02;
    
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
                
                updateColumnIndex();
                
                //cout << "Angle = " << angle << "\n";
                //sendTestAngle();
                
                if( isSendingStripe01 )
                    sendStripeForAngleTo01();
                if( isSendingStripe02 )
                    sendStripeForAngleTo02();
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
        ofRect(  x + colum01 - 1 , y - 1 , 3 , outputImage->height + 2);
        ofRect(  x + colum02 - 1 , y - 1 , 3 , outputImage->height + 2);
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
    
private:
    void sendTestAngle(){
        if( !opcClient.isConnected() )
            return;
        colorsStrip01.clear();
        
        float c1 = ofMap( angle01 , 0 , 2.0 * PI , 0 , 255 );
        
        for( int p = 0 ; p < outputImage->height ; p ++ ){
            ofColor pixelColor = ofColor( c1 , 255 , 255 - c1 );
            colorsStrip01.push_back( pixelColor );
        }
        
        opcClient.writeChannel( indexStripe01 , colorsStrip01 );
    }
    //--------------------------------------------------------------
    void updateColumnIndex(){
        angle01 = sensor->getValue01();
        angle02 = angle01 + PI;
        if( angle02 >= 2 * PI )
            angle02 = fmod( angle02 , 2.0 * PI );
        
        colum01 = ofMap( angle01 , 0 , 2 * PI , 0 , outputImage->width-1  );
        colum02 = ofMap( angle02 , 0 , 2 * PI , 0 , outputImage->width-1  );
        
        //            if( angle <= PI )
        //                colum = ofMap( angle , 0 , PI , 0 , outputImage->width-1  );
        //            else
        //                colum = ofMap( angle , PI , 2.0 * PI ,  outputImage->width-1 , 0 );
        //            if( angle2 <= PI )
        //                colum = ofMap( angle2 , 0 , PI , 0 , outputImage->width-1  );
        //            else
        //                colum = ofMap( angle2 , PI , 2.0 * PI ,  outputImage->width-1 , 0 );
        //

    }
    //--------------------------------------------------------------
    void sendStripeForAngleTo01(){
        if( !opcClient.isConnected() )
            return;
        if( pixelsOutput ){
            colorsStrip01.clear();
            for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                int pixelIndex =  outputImage->width * pixel + colum01;
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
            for( int pixel = 0 ; pixel < outputImage->height ; pixel ++ ){
                int pixelIndex =  outputImage->width * pixel + colum02;
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
