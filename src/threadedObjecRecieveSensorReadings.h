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
    double value01;
    double value02;
    double value03;
    
private:
    void recieveMessages( ){
        foundBegining = false;
        foundEnd = false;
        
        while( !foundBegining ){
            recieved.clear();
            byteReaded = serialToArduino->readByte();
            if( byteReaded == '<'){
                foundBegining = true;
                //cout << "begin : ";
                while( !foundEnd ){
                    if( serialToArduino->available() ){
                        byteReaded = serialToArduino->readByte();
                        if( byteReaded ){
                            if( byteReaded != '>' && byteReaded != '<' )
                                recieved += byteReaded;
                            else if( byteReaded == '>' ){
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
            if( vstrings.size() == 1 )
                value01 = ofToDouble( vstrings[0] );
        }
//        
//        if( foundBegining && foundEnd  ){
//            std::stringstream ss(recieved);
//            std::istream_iterator<std::string> begin(ss);
//            std::istream_iterator<std::string> end;
//            std::vector<std::string> vstrings(begin, end);
//            if( vstrings.size() == 3 ){
//                value01 = 0.0174532925 * ofToDouble( vstrings[0] );
//                value02 = 0.0174532925 * ofToDouble( vstrings[1] );
//                value03 = 0.0174532925 * ofToDouble( vstrings[2] );
//            }
//        }
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
                //cout << "euler = ( " << value01 << " , " << value02 << " , " << value03 << " ) " << "\n";
                
                unlock();
            }
            else
                cout << "threadedFunction()" << "Unable to lock mutex." << "\n";
        }
    }
    
    double getValue01(){
        ofScopedLock lock(mutex);
        return value01;
    }
//    double getValue02(){
//        ofScopedLock lock(mutex);
//        return value02;
//    }
//    double getValue03(){
//        ofScopedLock lock(mutex);
//        return value03;
//    }
};
