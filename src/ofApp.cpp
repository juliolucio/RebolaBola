#include "ofApp.h"
extern "C" {
#include "gamma.h"
}
//--------------------------------------------------------------
void ofApp::setup(){
    //openChildApp();
    
    ofSetVerticalSync(true);
    
    isRetina = true;
    numPixels = 64;
    
    imageInput = new ofImage();
    pixelator = new ofxImagePixelator();
    imageOutput = pixelator->getOutputImage();
    
    screenSelectionCapture.setup( isRetina, ofPoint( 300 , 300 ), ofPoint( 200 , 200 ) );
    backgroudCapture.setup( isRetina );
    
    projectableImage = new ofxProjectableImage();
    projectableImage->setup();
    
    setCaptureType( CAPTURE_FROM_IMAGE_FILE , "colorGradienX.png" );
    
    ofBackground( 0 );
    
    setupGuiInput();
    setupGuiDebug();
    setupGuiOutput();
    
    threadSensorReciver = new ThreadedObjecRecieveSensorReadings();
    threadSensorReciver->setup( "/dev/cu.usbmodem1421" , 115200 );
    threadSensorReciver->start();
    assert( threadSensorReciver->isConectedToSensor() );
    
    threadLedSender.setOutputImage( imageOutput );
    threadLedSender.setSensorThread( threadSensorReciver );
    threadLedSender.start();
    
}
//--------------------------------------------------------------
void ofApp::update(){
    if( isDebuging )
        updateTests();
    
    else{
        switch(captureType){
            case CAPTURE_UNSET:
                return;
            case CAPTURE_FROM_IMAGE_FILE:
                break;
                
            case CAPTURE_FROM_SCREEN:
                backgroudCapture.update();
                screenSelectionCapture.update();
                imageInput->setFromPixels( screenSelectionCapture.getImage()->getPixels() , screenSelectionCapture.getImage()->getWidth(), screenSelectionCapture.getImage()->getHeight(), OF_IMAGE_COLOR_ALPHA );
                break;
                
            case CAPTURE_FROM_VIDEO_FILE:
                video.update();
                imageInput->setFromPixels( video.getPixels() , video.getWidth(), video.getHeight(), OF_IMAGE_COLOR );
                break;
                
            case CAPTURE_FROM_CAMERA:
                camera.update();
                imageInput->setFromPixels( camera.getPixels() , camera.getWidth() , camera.getHeight(), OF_IMAGE_COLOR );
                break;
        }
        pixelator->update();
        imageOutput = pixelator->getOutputImage();
        //test();
        updateGamma();
        updateAtenuation();
    }
    //threadLedSender.sendTestAngle();
    setDrawingStripe01();
    setDrawingStripe02();
    //threadLedSender.setDelay( updateDelay );
}
//--------------------------------------------------------------
void ofApp::setCaptureType( captuteTypes type , string fileName ){
    captureType = type;
    video.stop();
    imageInput->clear();
    switch (captureType) {
        case CAPTURE_UNSET:
            return;
        case CAPTURE_FROM_IMAGE_FILE:
            assert( imageInput->loadImage( fileName ));
            //            if( !imageInput->loadImage( fileName ) ){
            //                cout << "**error: flie " <<  fileName << "not found\n";
            //                break;
            //            }
            break;
            
        case CAPTURE_FROM_SCREEN:
            imageInput->allocate( screenSelectionCapture.getImage()->getWidth() , screenSelectionCapture.getImage()->getHeight(), OF_IMAGE_COLOR_ALPHA );
            //imageInput = screenSelectionCapture.getImage();
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            //assert( !video.loadMovie( fileName ) );
            if( !video.loadMovie( fileName ) ){
                cout << "**error: flie " <<  fileName << "not found\n";
                break;
            }
            imageInput->allocate( video.getWidth() , video.getHeight(), OF_IMAGE_COLOR );
            video.play();
            break;
            
        case CAPTURE_FROM_CAMERA:
            //assert( !camera.initGrabber( 320 , 240 ) );
            if( !camera.initGrabber( 320 , 240 ) ){
                cout << "**error: camera not found\n";
                break;
            }
            imageInput->allocate( camera.getWidth() , camera.getHeight(), OF_IMAGE_COLOR );
            break;
    }
    pixelator->setImage( imageInput , numPixels );
    cout << " setting capture mode to  " <<  captureType << "\n";
    
}
////--------------------------------------------------------------
//    void ofApp::setProjection( ofVec3f theImagePosition , ofVec3f theImageNormal ){
//    projectableImage->setup();
//    cout    << "Reseting sizes capture mode to  " <<  captureType << "\n"
//    << "\tcapture width = " <<  imageOutput->getWidth() << "," << "\n"
//    << "\tcapture height = " <<  imageOutput->getHeight() << "\n" << "\n";
//}
//--------------------------------------------------------------
void ofApp::updateTests(){
    unsigned char* pixelsOutput = imageOutput->getPixels();
    for( int y = 0 ; y <  imageOutput->getHeight()  ; y ++){
        for( int x = 0 ; x < imageOutput->getWidth()  ; x ++){
            ofColor testColor;
            
            if( isDebugTestX ){
                if( x < imageOutput->getWidth() / 2 ){
                    testColor.r = 255;
                    testColor.g = 0;
                    testColor.b = 0;
                }
                else{
                    testColor.r = 0;
                    testColor.g =0;
                    testColor.b = 255;
                }
            }
            
            else if( isDebugTestY ){
                if( y < imageOutput->getHeight() / 2 ){
                    testColor.r = 255;
                    testColor.g = 0;
                    testColor.b = 0;
                }
                else{
                    testColor.r = 0;
                    testColor.g = 0;
                    testColor.b = 255;
                }
            }
            
            else if( isDebugisDebugTestXY ){
                testColor.r  = ofMap( y , 0 , imageOutput->getHeight()   , 0 , 255 );
                testColor.b  = ofMap( y , 0 , imageOutput->getHeight()    , 255 , 0 );
                testColor.g  = ofMap( x , 0 , imageOutput->getWidth()    , 0 , 255 );
            }
            imageOutput->setColor( x , y , testColor );
        }
    }
    imageOutput->update();
}
//--------------------------------------------------------------
void ofApp::updateAtenuation(){
    if( !isAtenuated )
        return;
    unsigned char* pixelsOutput = imageOutput->getPixels();
    for( int p = 0 ; p < imageOutput->getWidth() * imageOutput->getHeight() * 3 ; p ++ )
        pixelsOutput[p] = float(pixelAtenuation) * float( pixelsOutput[p] );
    imageOutput->setFromPixels( pixelsOutput , imageOutput->getWidth() , imageOutput->getHeight(), OF_IMAGE_COLOR );
    imageOutput->update();
}
//--------------------------------------------------------------
void ofApp::test(){
    unsigned char* pixelsOutput = imageOutput->getPixels();
    for( int p = 0 ; p < imageOutput->getWidth() * imageOutput->getHeight()  ; p ++ ){
        pixelsOutput[3*p] = 100;
        pixelsOutput[3*p+1] = 0;
        pixelsOutput[3*p+2] = 0;
    }
    imageOutput->setFromPixels( pixelsOutput , imageOutput->getWidth() , imageOutput->getHeight(), OF_IMAGE_COLOR );
    imageOutput->update();
}
//--------------------------------------------------------------
void ofApp::updateGamma(){
    if( !isGammaCorrecting )
        return;
    unsigned char* pixelsOutput = imageOutput->getPixels();
    for( int p = 0 ; p < imageOutput->getWidth() * imageOutput->getHeight() * 3 ; p ++ )
        pixelsOutput[ p ] = gammaCorrection[ pixelsOutput[ p ] ];
    imageOutput->setFromPixels( pixelsOutput , imageOutput->getWidth() , imageOutput->getHeight(), OF_IMAGE_COLOR );
    imageOutput->update();
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255);
    ofClear(127);
    if( captureType == CAPTURE_FROM_SCREEN )
        backgroudCapture.draw();
    
    //if( ofGetElapsedTimeMillis() - lastTimerUserInteracted < 7000 ){
    ofPoint drawPos = ofPoint( 550 , 20 );
    drawInput( drawPos.x , drawPos.y );
    drawOutput( drawPos.x , imageInput->getHeight() + drawPos.y * 2 );
    
    guiInput.draw();
    guiDebug.draw();
    guiOutput.draw();
    
    if( captureType ==  CAPTURE_FROM_SCREEN ){
        screenSelectionCapture.draw();
    }
    threadLedSender.drawLastColumStripe01( ofGetWidth() - 80  , 0 , 4  );
    threadLedSender.drawLastColumStripe02( ofGetWidth() - 40  , 0 , 4  );
    threadLedSender.drawAngle( ofPoint( 300 , 20  ) , ofPoint( 100 , 100  ));
    threadLedSender.drawOutput( 300 , 200  );
    //}
    
    //drawing the mouse cursor
    ofSetHexColor( 0xff2222 );
    ofNoFill();
    ofCircle( ofGetMouseX() , ofGetMouseY() , 4 );
    ofFill();
    
    ofSetHexColor( 0x22ff22f );
    ofCircle( ofGetMouseX() , ofGetMouseY() , 3 );
    ofSetHexColor( 0x2222ff );
    ofCircle( ofGetMouseX() , ofGetMouseY() , 2 );
}
//--------------------------------------------------------------
void ofApp::drawOutput( int x , int y ){
    int borderSize = 8;
    ofSetColor( 120 , 120 , 180 );
    ofFill();
    ofRect( x - borderSize , y - borderSize , imageOutput->getWidth() + 2 * borderSize , imageOutput->getHeight() + 2 * borderSize );
    
    ofSetColor( 50 , 50, 90 );
    ofRect( x - borderSize / 2 , y - borderSize / 2 , imageOutput->getWidth() + 2 * borderSize / 2 , imageOutput->getHeight() + 2 * borderSize / 2 );
    ofSetColor(255);
    ofNoFill();
    imageOutput->draw( x , y , imageOutput->getWidth() , imageOutput->getHeight() );
    ofDrawBitmapString( "Output", ofPoint( x + 5 , y + 2 ) );
}
//--------------------------------------------------------------
void ofApp::drawLastColum( int x , int y ,  int widthFactor ){
    int width = ofGetHeight() / imageOutput->getHeight();
    unsigned char *pixelsOutput = imageOutput->getPixels();
    for( int i = 0 ; i < imageOutput->getHeight() ; i ++ ){
        ofFill();
        ofSetHexColor( 0x229922 );
        ofRect( x , y + width * i , widthFactor * width , width  );
        
        if( !(i % 10) )
            ofDrawBitmapString(ofToString(i), ofPoint( x + width ,y + width * i) );
        
        //int pixelIndex =  0 + outputWidth * i;
        int pixelIndex =  imageOutput->getWidth() * i;
        ofSetColor( ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] ) );
        ofRect( x+2 , y + width * i + 2 , widthFactor * width - 2 , width - 2  );
        ofSetColor(255);
    }
}
//--------------------------------------------------------------
void ofApp::drawInput( int x , int y  ){
    int borderSize = 8;
    ofSetColor( 180 , 200 , 10 );
    ofFill();
    ofRect( x - borderSize , y - borderSize , imageInput->getWidth() + 2 * borderSize , imageInput->getHeight() + 2 * borderSize );
    
    ofSetColor( 50 , 50, 90 );
    ofRect( x - borderSize / 2 , y - borderSize / 2 , imageInput->getWidth() + 2 * borderSize / 2 , imageInput->getHeight() + 2 * borderSize / 2 );
    ofSetColor(255);
    ofNoFill();
    imageInput->draw( x , y , imageInput->getWidth() , imageInput->getHeight() );
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch( key ){
        case '1':
            setInputModeImage();
            break;
            
        case '2':
            setInputModeVideo();
            break;
            
        case '3':
            setInputModeCamera();
            break;
            
        case '4':
            setInputModeScreen();
            break;
            
        default:
            break;
    }
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 's') {
        guiDebug.saveToFile("settingsDebug.xml");
        guiInput.saveToFile("settingsInput.xml");
        guiOutput.saveToFile("settingsOutput.xml");
    }
    if(key == 'l') {
        guiDebug.loadFromFile("settingsDebug.xml");
        guiInput.loadFromFile("settingsInput.xml");
        guiOutput.loadFromFile("settingsOutput.xml");
    }
    
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    screenSelectionCapture.mouseMoved(x, y);
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    screenSelectionCapture.mouseDragged(x, y , button) ;
    
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    screenSelectionCapture.mousePressed(x, y , button) ;
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    screenSelectionCapture.mouseReleased(x, y , button) ;
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
void ofApp::openChildApp(){
    string shPath;
    shPath = ofToDataPath( "openChildApp.sh", true );
    
    char *shPathChar;
    shPathChar = new char[ shPath.length() + 1 ];
    
    strcpy( shPathChar, shPath.c_str() );
    int pid = fork();
    
    cout << "pid :: " << pid << endl;
    switch ( pid ){
        case -1 :
            cout << "Uh-Oh! fork() failed.\n" << endl;
            break;
        case  0 :
            execl( shPathChar, shPathChar, NULL );
            break;
        default :
            break;
    }
}

