#pragma once


#include "ofThread.h"
#include "ofxOPC.h"

class ThreadedObjecSendOutputToLedStripe: public ofThread{
private:
    ofxOPC opcClient;
    
    int indexStripe0;
    int indexStripe1;

    
    std::vector<ofColor> colorsStripe01;
    std::vector<ofColor> colorsStripe02;

    void sendStripe(){
        opcClient.writeChannel( indexStripe1 , colorsStripe01 );
    }


public:
    ThreadedObjecSendOutputToLedStripe(){
        opcClient.setup("127.0.0.1", 7890);
        indexStripe0 = 1;
        indexStripe1 = 8;

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
                sendStripe();
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void setOutputStripe01( std::vector<ofColor> &theColorStripe){
        if(lock()){
            colorsStripe01 = theColorStripe;
            //cout << colorsStripe01[31];
            unlock();
        }
        
        else{
            cout << "setOutputImage()" << "Unable to lock mutex." << "\n";
        }
    }
    
    void setOutputStripe02( std::vector<ofColor> &theColorStripe){
        if(lock()){
            colorsStripe02 = theColorStripe;
            unlock();
        }
        
        else{
            cout << "setOutputImage()" << "Unable to lock mutex." << "\n";
        }
    }
};