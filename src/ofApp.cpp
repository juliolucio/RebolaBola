#include "ofApp.h"
extern "C" {
#include "gamma.h"
}
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    isRetina = true;
    numPixels = 64;
    
    imageInput = new ofImage();
    pixelator = new ofxImagePixelator();

    screenSelectionCapture.setup( isRetina, ofPoint( 100 , 100 ), ofPoint( 64 , 64 ) );
    backgroudCapture.setup( isRetina );
    
    projectableImage = new ofxProjectableImage();
    projectableImage->setup();

    setCaptureType( CAPTURE_FROM_IMAGE_FILE , "colors.jpg" );
    //setCaptureType( CAPTURE_FROM_SCREEN , ":)" );
    //setCaptureType( CAPTURE_FROM_VIDEO_FILE , "video01.mov" );
    //setCaptureType( CAPTURE_FROM_CAMERA , ":)" );
    
    ofBackground( 0 );
    
    setupGuiInput();
    setupGuiDebug();
    setupGuiOutput();

    threadLedSender.start();
    //threadLedSender.setOutputColumDelay( columDrawingDelay );
    
    threadSensorReciver.start();
    threadSensorReciver.setup( "/dev/cu.usbmodem1451" , 115200 );
}
//--------------------------------------------------------------
void ofApp::update(){
    switch(captureType){
        case CAPTURE_FROM_IMAGE_FILE:
            break;

        case CAPTURE_FROM_SCREEN:
            backgroudCapture.update();
            screenSelectionCapture.update();
            break;
    
        case CAPTURE_FROM_VIDEO_FILE:
            video.update();
            break;
    
        case CAPTURE_FROM_CAMERA:
            camera.update();
            break;
            
        default:
            break;
    }
    screenSelectionCapture.update();
    pixelator->update();
    imageOutput = pixelator->getOutputImage();
    
    
    float angleTeta = threadSensorReciver.getAngleBetta();

    angleTeta = angleTeta * PI / 180;
    cout << " angle = " << angleTeta << "\n";
    projectableImage->updateAngleTeta( angleTeta );
    projectableImage->setImage( imageInput );
    projectableImage->updateSphereColor();
    std::vector<ofColor> stripe;
    ofxNeoPixelLedArc* tempArc = projectableImage->sphere->arcs[0];
    for( int i = 0 ; i < tempArc->leds.size() ; i ++ )
        stripe.push_back(  tempArc->leds[i]->currentColor );
    threadLedSender.setOutputStripe01( stripe );
}
//--------------------------------------------------------------
void ofApp::setCaptureType( captuteTypes type , string fileName ){
    captureType = type;
    video.close();
    imageInput->clear();
    switch (captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            if( !imageInput->loadImage( fileName ) ){
                cout << "**error: flie " <<  fileName << "not found\n";
                break;
            }
            break;
            
        case CAPTURE_FROM_SCREEN:
            imageInput->allocate( screenSelectionCapture.getSize().x , screenSelectionCapture.getSize().y , OF_IMAGE_COLOR );
            break;

        case CAPTURE_FROM_VIDEO_FILE:
            if( !video.loadMovie( fileName ) ){
                cout << "**error: flie " <<  fileName << "not found\n";
                break;
            }
            imageInput->allocate( video.width , video.height, OF_IMAGE_COLOR );
            imageInput->setFromPixels( video.getPixels() , video.width , video.height, OF_IMAGE_COLOR );
            video.play();
            break;
            
        case CAPTURE_FROM_CAMERA:
            if( !camera.initGrabber( 320 , 240 ) ){
                cout << "**error: camera not found\n";
                break;
            }
            imageInput->allocate( video.width , video.height, OF_IMAGE_COLOR );
            imageInput->setFromPixels( camera.getPixels() , camera.width , camera.height, OF_IMAGE_COLOR );
            break;
    }
    pixelator->setImage( imageInput , numPixels , isRetina );
    cout << " setting capture mode to  " <<  captureType << "\n";
}
//--------------------------------------------------------------
    void ofApp::setProjection( ofVec3f theImagePosition , ofVec3f theImageNormal ){
//    ofVec3f position = ofVec3f( 1 , 0 , 0 );
//    ofVec3f normal = ofVec3f( -1 , 0 , 0 );

    projectableImage->setup();
    //threadLedSender.setOutputSize( ofPoint( imageOutput->width , imageOutput->height ) );
    
    cout    << "Reseting sizes capture mode to  " <<  captureType << "\n"
    << "\tcapture width = " <<  imageOutput->width << "," << "\n"
    << "\tcapture height = " <<  imageOutput->height << "\n" << "\n";
}
//--------------------------------------------------------------
void ofApp::updateTests(){
    unsigned char* pixelsOutput = imageOutput->getPixels();
    for( int y = 0 ; y <  imageOutput->height  ; y ++){
        for( int x = 0 ; x < imageOutput->width  ; x ++){
            ofColor testColor;
            
            if( isDebugTestX ){
                if( x < imageOutput->width / 2 ){
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
                if( y < imageOutput->height / 2 ){
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
                testColor.r  = ofMap( y , 0 , imageOutput->height   , 0 , 255 );
                testColor.b  = ofMap( y , 0 , imageOutput->height    , 255 , 0 );
                testColor.g  = ofMap( x , 0 , imageOutput->width    , 0 , 255 );
            }
            
            if( isAtenuated ){
                testColor.r *= pixelAtenuation;
                testColor.g *= pixelAtenuation;
                testColor.b *= pixelAtenuation;
            }
            
            if( isGammaCorrecting ){
                testColor.r = gammaCorrection[testColor.r];
                testColor.g = gammaCorrection[testColor.g];
                testColor.b = gammaCorrection[testColor.b];
            }
            
            testColor.r = min( int(testColor.r) , 254 );
            testColor.g = min( int(testColor.g), 254 );
            testColor.b = min( int(testColor.b), 254 );
            
            testColor.r = max( int(testColor.r), 0 );
            testColor.g = max( int(testColor.g), 0 );
            testColor.b = max( int(testColor.b), 0 );
            
            imageOutput->setColor( x , y , testColor );
        }
    }
    imageOutput->update();
}
//--------------------------------------------------------------
void ofApp::drawOutput( int x , int y , int width , int height ){
    int borderSize = 8;
    ofSetColor( 120 , 120 , 180 );
    ofFill();
    ofRect( x - borderSize , y - borderSize , width + 2 * borderSize , height + 2 * borderSize );
    
    ofSetColor( 50 , 50, 90 );
    ofRect( x - borderSize / 2 , y - borderSize / 2 , imageOutput->width + 2 * borderSize / 2 , imageOutput->height + 2 * borderSize / 2 );
    ofSetColor(255);
    ofNoFill();
    imageOutput->draw( x , y , width , height );
    ofDrawBitmapString( "Output", ofPoint( x + 5 , y + 2 ) );
}
//--------------------------------------------------------------
void ofApp::drawLastColum( int x , int y ,  int stripeIndex ){
    int width = ofGetHeight() / imageOutput->height;
    unsigned char *pixelsOutput = imageOutput->getPixels();
    for( int i = 0 ; i < imageOutput->height ; i ++ ){
        ofFill();
        ofSetHexColor( 0x229922 );
        ofRect( x , y + width * i , width , width  );
        
        if( !(i % 10) )
            ofDrawBitmapString(ofToString(i), ofPoint( x + width ,y + width * i) );
        
        //int pixelIndex =  0 + outputWidth * i;
        int pixelIndex =  imageOutput->width * i;
        ofSetColor( ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] ) );
        ofRect( x+2 , y + width * i + 2 , width - 2 , width - 2  );
        ofSetColor(255);
    }
}
//--------------------------------------------------------------
void ofApp::drawInputResul( int x , int y , int width , int height ){
    int borderSize = 8;
    ofSetColor( 180 , 200 , 10 );
    ofFill();
    ofRect( x - borderSize , y - borderSize , width + 2 * borderSize , height + 2 * borderSize );
    
    ofSetColor( 50 , 50, 90 );
    ofRect( x - borderSize / 2 , y - borderSize / 2 , width + 2 * borderSize / 2 , height + 2 * borderSize / 2 );
    ofSetColor(255);
    ofNoFill();
    imageInput->draw( x , y , width , height );
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255);
    ofClear(127);
    if( captureType == CAPTURE_FROM_SCREEN )
        backgroudCapture.draw();
    
    drawLastColum( 0 , 0 , 0 );
    if( ofGetElapsedTimeMillis() - lastTimerUserInteracted < 7000 ){
        drawOutput( 550 , 20 , imageOutput->width , imageOutput->height );
        drawInputResul( 550 , 100 , imageInput->width , imageInput->height );
        pixelator->draw();
        guiInput.draw();
        guiDebug.draw();
        guiOutput.draw();
        //projectableImage->draw( 200 , 200 );
        
        drawLastColum( 0 , 0 , 0 );
        
        if( captureType ==  CAPTURE_FROM_SCREEN ){
            screenSelectionCapture.draw();
        }
    }
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
void ofApp::keyPressed(int key){
    switch( key ){
        case '1':
            setCaptureType( CAPTURE_FROM_IMAGE_FILE , "image01.png" );
            break;
            
        case '2':
            setCaptureType( CAPTURE_FROM_SCREEN , ":)" );
            break;
            
        case '3':
            setCaptureType( CAPTURE_FROM_VIDEO_FILE , "video0101.mov" );
            break;
            
        case '4':
            setCaptureType( CAPTURE_FROM_CAMERA , ":)" );
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
