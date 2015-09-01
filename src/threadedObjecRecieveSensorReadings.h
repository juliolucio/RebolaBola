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
    float           angleTeta;
    
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
        int recivedSize = recieved.size();
        if( foundBegining && foundEnd && recivedSize > 5 ){
            angleTeta = ofToFloat( recieved );
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
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    float getAngleTeta(){
        ofScopedLock lock(mutex);
        return angleTeta;
    }
};
