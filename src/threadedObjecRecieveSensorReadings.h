#pragma once

#include "ofThread.h"

class ThreadedObjecRecieveSensorReadings: public ofThread{
private:
    ofSerial*       serialToArduino;
    string          recieved;
    unsigned char byteReaded;
    bool foundBegining;
    bool foundEnd;
    
protected:
    double angleAlpha;
    double angleBeta;
    double angleGamma;
    
private:
    void recieveMessages( ){
        foundBegining = false;
        foundEnd = false;
        
        while( !foundBegining ){
            recieved.clear();
            byteReaded = serialToArduino->readByte();
            if( byteReaded == 'a'){
                foundBegining = true;
                //cout << "begin : ";
                while( !foundEnd ){
                    if( serialToArduino->available() ){
                        byteReaded = serialToArduino->readByte();
                        if( byteReaded ){
                            if( byteReaded != 'b' && byteReaded != 'a' )
                                recieved += byteReaded;
                            else if( byteReaded == 'b' ){
                                foundEnd = true;
                                //cout << "  end\n";
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
                angleAlpha = ofToDouble( vstrings[0] );
                angleBeta = ofToDouble( vstrings[1] );
                angleGamma = ofToDouble( vstrings[2] );
             }
        }
    }
    
    
public:
    ThreadedObjecRecieveSensorReadings() {
    }
    
    void setup( std::string serialPort , int sppedInBauds ){
        serialToArduino = new ofSerial();
        serialToArduino->listDevices();
        vector <ofSerialDeviceInfo> deviceList = serialToArduino->getDeviceList();
        serialToArduino->setup(serialPort , sppedInBauds); //open the first device
    }
    
    bool isConectedToSensor(){
        ofScopedLock lock(mutex);
            if( serialToArduino->isInitialized() )
                return true;
        return false;
        
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
                if( serialToArduino->isInitialized() )
                recieveMessages();
                //sleep(1);
                //cout << "euler = ( " << angleAlpha << " , " << angleBeta << " , " << angleGamma << " ) " << "\n";
                
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    double getAngleAlpha(){
        ofScopedLock lock(mutex);
        return angleAlpha;
    }
    double getAngleBeta(){
        ofScopedLock lock(mutex);
        return angleBeta;
    }
    double getAngleGamma(){
        ofScopedLock lock(mutex);
        return angleGamma;
    }
};
