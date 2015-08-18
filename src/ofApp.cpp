#include "ofApp.h"
extern "C" {
#include "macGlutfix.h"
#include "gamma.h"
}
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    retina = true;
    lastColunSnedStripe01 = 0;
    lastColunSnedStripe02 = 0;
    
    indexStripe0 = 1;
    indexStripe1 = 8;
    
    int numPixels = 64;
    outputHeight = numPixels;
    
    //setCaptureType( CAPTURE_FROM_IMAGE_FILE , "pano.jpeg" );
    setCaptureType( CAPTURE_FROM_SCREEN , ":)" );
    //setCaptureType( CAPTURE_FROM_VIDEO_FILE , "video01.mov" );
    //setCaptureType( CAPTURE_FROM_CAMERA , ":)" );
    
    updateIndex = 0;
    ofBackground( 0 );
    
    opcClient.setup("127.0.0.1", 7890);
    lastTimeSendtripe01 = 0;
    lastTimeSendtripe02 = 0;
    
    linesPerSecond.addListener(this, &ofApp::setPixelLineRate );
    
    int retinalFactor = 1;
    if( retina  )
        retinalFactor = 2;
    screenBackground.allocate( retinalFactor * ofGetWidth(), retinalFactor * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
    
    gui.setup();
    gui.add( linesPerSecond.setup("lines per second ", 400 , 10 , 10000 ) );
    gui.add( pixelAtenuation.setup("Pixel Atenuation", .6 , 0 , 1 ) );
    
    gui.add( testX.setup("test X" , false ) );
    gui.add( testY.setup("test Y" , false ) );
    gui.add( testXY.setup("test XY" , false ) );
    gui.add( isGammaCorrecting.setup("Gamma correction" , false ) );
    gui.add( isAtenuated.setup("Atenuated" , false ) );
    
    gui.add( updatingStripe01.setup("Draw Stripe 01" , false ) );
    gui.add( updatingStripe02.setup("Draw Stripe 02" , false ) );
    gui.add(  updatingAlternating.setup("Alterning stripes" , false ) );
    
    gui.loadFromFile("settings.xml");
    
    ofSetLogLevel( OF_LOG_VERBOSE );
}
//--------------------------------------------------------------
void ofApp::setCaptureType( captuteTypes type , string fileName ){
    captureType = type;
    imageFromFile.clear();
    video.close();
    
    switch (captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            if( !imageFromFile.loadImage( fileName ) ){
                cout << "**error: flie " <<  fileName << "not found\n";
                break;
            }
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            if( !video.loadMovie( fileName ) ){
                cout << "**error: flie " <<  fileName << "not found\n";
                break;
            }
            video.play();
            break;
            
        case CAPTURE_FROM_CAMERA:
            if( !camera.initGrabber( 320 , 240 ) ){
                cout << "**error: camera not found\n";
                break;
            }
            break;
            
        default:
            break;
    }
    
    if( retina && captureType == CAPTURE_FROM_SCREEN )
        rx = 2;
    else
        rx = 1;
    
    cout << " setting capture mode to  " <<  captureType << "\n";
    resetSizes();
}
//--------------------------------------------------------------
void ofApp::setPixelLineRate( int & lineRate ){
    micrisecondsBetwenLineUpdate = 1000000 / lineRate;
}
//--------------------------------------------------------------
void ofApp::resetSizes(){
    int captureRestWidth;
    int captureRestHeight;
    float proportion;
    
    switch(captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            proportion = imageFromFile.width / imageFromFile.height;
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = imageFromFile.width % outputWidth;
            captureRestHeight = imageFromFile.height % outputHeight;
            
            captureWidth = imageFromFile.width - captureRestWidth;
            captureHeight = imageFromFile.height - captureRestHeight;
            break;
            
        case CAPTURE_FROM_SCREEN:
            proportion = selector.selectionSize.x / selector.selectionSize.y;
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = int(selector.selectionSize.x) % outputWidth;
            captureRestHeight = int(selector.selectionSize.y) % outputHeight;
            
            captureWidth = selector.selectionSize.x - captureRestWidth;
            captureHeight = selector.selectionSize.y - captureRestHeight;
            
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            proportion = video.width / video.height;
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = video.width % outputWidth;
            captureRestHeight = video.height % outputHeight;
            
            captureWidth = video.width - captureRestWidth;
            captureHeight = video.height - captureRestHeight;
            break;
            
        case CAPTURE_FROM_CAMERA:
            proportion = camera.width / camera.height;
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = camera.width % outputWidth;
            captureRestHeight = camera.height % outputHeight;
            
            captureWidth = camera.width - captureRestWidth;
            captureHeight = camera.height - captureRestHeight;
            break;
            
        default:
            break;
    }
    numPixelsCapturePerOutputWidth = ( rx * captureWidth ) / outputWidth ;
    numPixelsCapturePerOutputHeight = ( rx * captureHeight )  / outputHeight;
    
    inputImageResult.clear();
    inputImageResult.allocate( rx * captureWidth ,  rx * captureHeight , OF_IMAGE_COLOR_ALPHA ) ;
    
    output.clear();
    output.allocate( outputWidth , outputHeight , OF_IMAGE_COLOR_ALPHA );
    
    cout    << "Reseting sizes capture mode to  " <<  captureType << "\n"
    << "\tcapture width = " <<  captureWidth << "," << "\n"
    << "\tcapture height = " <<  captureHeight << "\n" << "\n";
}
//--------------------------------------------------------------
void ofApp::update(){
    updateImageBackground();
    selector.update(ofPoint( ofGetWindowPositionX() , ofGetWindowPositionY() ) );
    updateImageInput();
    updateImageOutput();
    tryToSendCplumToStripe01();
}
//--------------------------------------------------------------
void ofApp::updateImageBackground(){
    unsigned char * pixelsBackground = screenBackground.getPixels();
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int retinalFactor = 1;
    if( retina  )
        retinalFactor = 2;
    
    pixelsBackground = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , ofGetWidth()  , ofGetHeight() );
    for( int i = 0; i < rx * rx * ofGetWidth() * ofGetHeight() ; i++ ){
        r = pixelsBackground[ 4 * i + 1 ];
        g = pixelsBackground[ 4 * i + 2 ];
        b = pixelsBackground[ 4 * i + 3 ];
        a = pixelsBackground[ 4 * i + 0 ];
        
        pixelsBackground[ 4 * i + 0 ] = r;
        pixelsBackground[ 4 * i + 1 ] = g;
        pixelsBackground[ 4 * i + 2 ] = b;
        pixelsBackground[ 4 * i + 3 ] = a;
    }
    screenBackground.setFromPixels( pixelsBackground , retinalFactor * ofGetWidth(), retinalFactor * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
    screenBackground.update();
}
//--------------------------------------------------------------
void ofApp::updateImageInput(){
    unsigned char * pixelsInput;
    unsigned char * pixelsResult;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
    switch (captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            for( int x = 0 ; x < captureWidth ; x ++ )
                for( int y = 0 ; y < captureHeight ; y ++ )
                    inputImageResult.setColor( x , y , imageFromFile.getColor( x, y ) );
            break;
            
        case CAPTURE_FROM_SCREEN:
            pixelsInput = pixelsBelowWindow( ofGetWindowPositionX() + selector.position.x , ofGetWindowPositionY() + selector.position.y , captureWidth , captureHeight );
            
            for (int i = 0; i < rx  * rx * captureWidth * captureHeight; i++){
                r = pixelsInput[ 4 * i + 1 ];
                g = pixelsInput[ 4 * i + 2 ];
                b = pixelsInput[ 4 * i + 3 ];
                a = pixelsInput[ 4 * i + 0 ];
                
                pixelsInput[ 4 * i + 0 ] = r;
                pixelsInput[ 4 * i + 1 ] = g;
                pixelsInput[ 4 * i + 2 ] = b;
                pixelsInput[ 4 * i + 3 ] = a;
            }
            inputImageResult.setFromPixels( pixelsInput,  rx * captureWidth , rx * captureHeight , OF_IMAGE_COLOR_ALPHA );
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            video.update();
            pixelsInput = video.getPixels();
            for( int pixelIndex = 0 ; pixelIndex < video.width * video.height ; pixelIndex ++ ){
                int indexRed    = 3 * pixelIndex;
                int indexGreen  = 3 * pixelIndex + 1;
                int indexBlue   = 3 * pixelIndex + 2;
                int x = pixelIndex % video.width;
                int y = pixelIndex / video.width;
                if( x < captureWidth && y < captureHeight )
                    inputImageResult.setColor( x , y , ofColor( pixelsInput[ indexRed ], pixelsInput[ indexGreen] , pixelsInput[ indexBlue ] ) );
            }
            break;
            
        case CAPTURE_FROM_CAMERA:
            camera.update();
            pixelsInput = camera.getPixels();
            for( int pixelIndex = 0 ; pixelIndex < camera.width * camera.height ; pixelIndex ++ ){
                int indexRed    = 3 * pixelIndex;
                int indexGreen  = 3 * pixelIndex + 1;
                int indexBlue   = 3 * pixelIndex + 2;
                int x = pixelIndex % camera.width;
                int y = pixelIndex / camera.width;
                if( x < captureWidth && y < captureHeight )
                    inputImageResult.setColor( x , y , ofColor( pixelsInput[ indexRed ], pixelsInput[ indexGreen] , pixelsInput[ indexBlue ] ) );
            }
            break;
            
        default:
            break;
            
    }
    inputImageResult.update();
}
//--------------------------------------------------------------
void ofApp::updateImageOutput(){
    if( lastColunSnedStripe01 )
        return;
    pixelsOutput = output.getPixels();
    for( int y = 0 ; y <  outputHeight  ; y ++){
        for( int x = 0 ; x < outputWidth  ; x ++){
            ofPoint positionPixelIniCapture;
            positionPixelIniCapture.x = x * numPixelsCapturePerOutputWidth;
            positionPixelIniCapture.y = y * numPixelsCapturePerOutputHeight;
            
            int red = 0;
            int green = 0;
            int blue = 0;
            int numPix = 0;
            
            for( int pixeCaptureY = positionPixelIniCapture.y ; pixeCaptureY < positionPixelIniCapture.y  + numPixelsCapturePerOutputHeight ; pixeCaptureY ++ ){
                for( int pixeCaptureX = positionPixelIniCapture.x ; pixeCaptureX < positionPixelIniCapture.x  + numPixelsCapturePerOutputWidth ; pixeCaptureX ++){
                    ofColor currentPixelColor = inputImageResult.getColor( pixeCaptureX , pixeCaptureY );
                    red     += currentPixelColor.r;
                    green   += currentPixelColor.g;
                    blue    += currentPixelColor.b;
                    numPix++;
                }
            }
            
            red /= numPix;
            green /= numPix;
            blue /= numPix;
            
            if( testX ){
                if( x < output.width / 2 ){
                    red = 255;
                    green = 0;
                    blue = 0;
                }
                else{
                    red = 0;
                    green =0;
                    blue = 255;
                }
            }
            
            else if( testY ){
                if( y < output.height / 2 ){
                    red = 255;
                    green = 0;
                    blue = 0;
                }
                else{
                    red = 0;
                    green =0;
                    blue = 255;
                }
            }
            
            else if( testXY ){
                red     = ofMap( y , 0 , outputHeight   , 0 , 255 );
                blue    = ofMap( y , 0 , outputHeight    , 255 , 0 );
                green   = ofMap( x , 0 , outputWidth    , 0 , 255 );
            }
            
            if( isAtenuated ){
                red *= pixelAtenuation;
                green *= pixelAtenuation;
                blue *= pixelAtenuation;
            }
            
            if( isGammaCorrecting ){
                red = gammaCorrection[red];
                green = gammaCorrection[green];
                blue = gammaCorrection[blue];
            }
            
            red = min( red, 254 );
            green = min( green, 254 );
            blue = min( blue, 254 );
            
            red = max( red, 0 );
            green = max( green, 0 );
            blue = max( blue, 0 );
            
            output.setColor( x , y , ofColor( red , green , blue) ) ;
        }
    }
    output.update();
}
//--------------------------------------------------------------
void ofApp::drawOutput( int x , int y , int width , int height ){
    int borderSize = 8;
    ofSetColor( 120 , 120 , 180 );
    ofFill();
    ofRect( x - borderSize , y - borderSize , width + 2 * borderSize , height + 2 * borderSize );
    
    ofSetColor( 50 , 50, 90 );
    ofRect( x - borderSize / 2 , y - borderSize / 2 , output.width + 2 * borderSize / 2 , output.height + 2 * borderSize / 2 );
    ofSetColor(255);
    ofNoFill();
    output.draw( x , y , width , height );
}

//--------------------------------------------------------------
void ofApp::drawLastColum( int x , int y , int width , int stripeIndex ){
    for( int p = 0 ; p < outputHeight ; p ++ ){
        ofFill();
        ofSetHexColor( 0x22ff22 );
        ofRect( x , y + width * p , width , width  );
        
        int pixelIndex =  lastColunSnedStripe01 + outputHeight * p;
        ofSetColor( ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] ) );
        ofRect( x+2 , y + width * p + 2 , width - 2 , width - 2  );
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
    inputImageResult.draw( x , y , width , height );
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255);
    if( captureType == CAPTURE_FROM_SCREEN )
        screenBackground.draw(0,0, ofGetWidth() , ofGetHeight() );
    if( ofGetElapsedTimeMillis() - lastTimerUserInteracted < 7000 ){
        drawOutput( 430 , 20 , outputWidth , outputHeight );
        drawInputResul( 430 , 100 , inputImageResult.width , inputImageResult.height );
        gui.draw();
        drawLastColum( 0 , 0 , ofGetHeight() / outputHeight , 0 );
        if( captureType ==  CAPTURE_FROM_SCREEN ){
            ofNoFill();
            ofSetColor( 0 , 200 , 10 );
            ofRect( selector.position.x, selector.position.y, captureWidth, captureHeight);
            ofRect( selector.position.x, selector.position.y, selector.selectionSize.x, selector.selectionSize.y );
            
            selector.draw();
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
void ofApp::sendFrameToLedsDivided(){
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    for( int colum = 0 ; colum < outputWidth / 2 ; colum ++ ){
        colorsStrip01.clear();
        colorsStrip02.clear();
        for( int pixel = 0 ; pixel < outputHeight ; pixel ++ ){
            int pixelIndex01 = colum + outputWidth * pixel;
            int pixelIndex02 = ( colum + outputWidth / 2 ) + outputWidth * pixel;
            
            ofColor pixelColor01 = ofColor( pixelsOutput[ 3 * pixelIndex01 ] ,  pixelsOutput[ 3 * pixelIndex01 + 1 ] , pixelsOutput[ 3 * pixelIndex01  + 2 ] );
            ofColor pixelColor02 = ofColor( pixelsOutput[ 3 * pixelIndex02 ] ,  pixelsOutput[ 3 * pixelIndex02 + 1 ] , pixelsOutput[ 3 * pixelIndex02  + 2] );
            
            colorsStrip01.push_back(pixelColor01);
            colorsStrip02.push_back(pixelColor02);
        }
        
        if(opcClient.isConnected()){
            opcClient.writeChannel( indexStripe0, colorsStrip01 );
            opcClient.writeChannel( indexStripe1, colorsStrip02 );
        }
    }
}
//--------------------------------------------------------------
void ofApp::sendFrameToLedsSingle(){
    vector<ofColor> colorsStrip01;
    vector<ofColor> colorsStrip02;
    for( int colum = 0 ; colum < outputWidth ; colum ++ ){
        colorsStrip01.clear();
        colorsStrip02.clear();
        
        for( int pixel = 0 ; pixel < outputHeight ; pixel ++ ){
            int pixelIndex = colum + outputWidth * pixel;
            ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
            
            if( updatingStripe01 )
                colorsStrip01.push_back(pixelColor);
            else
                colorsStrip01.push_back(ofColor(0,0,0));
            
            
            if( updatingStripe02 )
                colorsStrip02.push_back(pixelColor);
            else
                colorsStrip02.push_back(ofColor(0,0,0));
        }
        
        if( opcClient.isConnected() ){
            opcClient.writeChannel( indexStripe0, colorsStrip01 );
            opcClient.writeChannel( indexStripe1, colorsStrip02 );
        }
    }
}
//--------------------------------------------------------------
void ofApp::tryToSendCplumToStripe01( ){
    int currenTime = ofGetElapsedTimeMicros();
    if( currenTime - lastTimeSendtripe01 > micrisecondsBetwenLineUpdate ){
        colorsStrip01.clear();
        for( int pixel = 0 ; pixel < outputHeight ; pixel ++ ){
            int pixelIndex =  lastColunSnedStripe01 + outputWidth * pixel;
            ofColor pixelColor = ofColor( pixelsOutput[ 3 * pixelIndex ] ,  pixelsOutput[ 3 * pixelIndex + 1 ] , pixelsOutput[ 3 * pixelIndex  + 2 ] );
            colorsStrip01.push_back( pixelColor );
        }
        
        if( opcClient.isConnected() ){
            lastTimeSendtripe01 = currenTime;
            opcClient.writeChannel( indexStripe0 , colorsStrip01 );
            lastColunSnedStripe01++;
            if( lastColunSnedStripe01 >= outputWidth )
                lastColunSnedStripe01 = 0;
        }
    }
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
        gui.saveToFile("settings.xml");
    }
    if(key == 'l') {
        gui.loadFromFile("settings.xml");
    }
    
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    selector.mouseMoved(x, y);
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    selector.mouseDragged(x, y , button) ;
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    selector.mousePressed(x, y , button) ;
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    lastTimerUserInteracted = ofGetElapsedTimeMillis();
    selector.mouseReleased(x, y , button) ;
    if( selector.isJustRelese )
        resetSizes();
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
