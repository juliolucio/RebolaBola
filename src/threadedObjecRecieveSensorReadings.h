#pragma once

#include "ofThread.h"

class threadedObjecRecieveSensorReadings: public ofThread{
private:
    ofSerial*       serialToArduino;
    string          recieved;
    unsigned char byteReaded;
    bool foundBegining;
    bool foundEnd;
    
protected:
    float angleAlpha;
    float angleBeta;
    float angleGamma;
    
private:
    void recieveMessages( ){
        foundBegining = false;
        foundEnd = false;
        
        while( !foundBegining ){
            recieved.clear();
            byteReaded = serialToArduino->readByte();
            if( byteReaded == 'a'){
                foundBegining = true;
                while( !foundEnd ){
                    if( serialToArduino->available() ){
                        byteReaded = serialToArduino->readByte();
                        if( byteReaded ){
                            if( byteReaded != 'b' && byteReaded != 'a' )
                                recieved += byteReaded;
                            else if( byteReaded == 'b' ){
                                foundEnd = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if( foundBegining && foundEnd  ){
            
            std::stringstream ss(recieved);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vstrings(begin, end);
            if( vstrings.size() == 3 ){
                angleAlpha = ofToFloat( vstrings[0] );
                angleBeta = ofToFloat( vstrings[1] );
                angleGamma = ofToFloat( vstrings[2] );
             }
        }
    }
    
    
public:
    threadedObjecRecieveSensorReadings() {
    }
    
    void setup( std::string serialPort , int sppedInBauds ){
        serialToArduino = new ofSerial();
        serialToArduino->listDevices();
        vector <ofSerialDeviceInfo> deviceList = serialToArduino->getDeviceList();
        serialToArduino->setup(serialPort , sppedInBauds); //open the first device
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
                recieveMessages();
                cout << "euler = ( " << angleAlpha << " , " << angleBeta << " , " << angleGamma << " ) " << "\n";
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    float getAngleAlpha(){
        ofScopedLock lock(mutex);
        return angleAlpha;
    }
    float getAngleBetta(){
        ofScopedLock lock(mutex);
        return angleBeta;
    }
    float getAngleGamma(){
        ofScopedLock lock(mutex);
        return angleGamma;
    }
};
