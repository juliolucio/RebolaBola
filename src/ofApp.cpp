#include "ofApp.h"
extern "C" {
#include "macGlutfix.h"
#include "gamma.h"
    
}
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    retina = true;
    
    int numPixels = 64;
    outputHeight = numPixels;
    
    selector.setup();
    selector.position = ofPoint( 230 , 10 );
    selector.selectionSize = ofPoint( 64 , 64 );
    
    sphere.setup( 2 , numPixels , 1 );
    projectableImage = new ofxProjectableImage();
    sphere.projectImage( projectableImage );
    
    //setCaptureType( CAPTURE_FROM_IMAGE_FILE , "pano.jpeg" );
    setCaptureType( CAPTURE_FROM_SCREEN , ":)" );
    //setCaptureType( CAPTURE_FROM_VIDEO_FILE , "video01.mov" );
    //setCaptureType( CAPTURE_FROM_CAMERA , ":)" );
    
    updateIndex = 0;
    ofBackground( 0 );
    
    int retinalFactor = 1;
    if( retina  )
        retinalFactor = 2;
    
    screenBackground.allocate( retinalFactor * ofGetWidth(), retinalFactor * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
    
    setupGuiInput();
    setupGuiDebug();
    setupGuiOutput();

    
    threadLedSender.start();
    threadLedSender.setOutputColumDelay( columDrawingDelay );
    
    threadSensorReciver.start();
    threadSensorReciver.setup( "/dev/cu.usbmodem1411" , 115200 );
}
//--------------------------------------------------------------
void ofApp::setupGuiInput(){
    guiInput.setup();
    guiInput.setPosition( 10 , 10 );
    guiInput.add( butonImage.setup( "Mode Load Image..." ) );
    guiInput.add( butonVideo.setup( "Mode Load Video..." ) );
    guiInput.add( butonCamera.setup( "Camera Mode" ) );
    guiInput.add( butonScreen.setup( "Capture form Screen" ) );
    guiInput.loadFromFile("settingsInput.xml");
    
    butonImage.addListener(this, &ofApp::setInputModeImage );
    butonVideo.addListener(this, &ofApp::setInputModeVideo );
    butonCamera.addListener(this, &ofApp::setInputModeCamera );
    butonScreen.addListener(this, &ofApp::setInputModeScreen );
}
//--------------------------------------------------------------
void ofApp::setupGuiDebug(){
    guiDebug.setup();
    guiDebug.setPosition( 10 , 120 );
    guiDebug.add( isDebuging.setup("Debug Mode" , false ) );
    guiDebug.add( isDebugTestX.setup("test X" , false ) );
    guiDebug.add( isDebugTestY.setup("test Y" , false ) );
    guiDebug.add( isDebugisDebugTestXY.setup("test XY" , false ) );
    guiDebug.loadFromFile("settingsDebug.xml");
}
//--------------------------------------------------------------
void ofApp::setupGuiOutput(){
    guiOutput.setup();
    guiOutput.setPosition( 10 , 230 );
    guiOutput.add( isAtenuated.setup("Atenuated" , false ) );
    guiOutput.add( pixelAtenuation.setup("Pixel Atenuation", .6 , 0 , 1 ) );
    guiOutput.add( isGammaCorrecting.setup("Gamma correction" , false ) );
    
    guiOutput.add( isUpdatingStripe01.setup("Draw Stripe 01" , false ) );
    guiOutput.add( isUpdatingStripe02.setup("Draw Stripe 02" , false ) );
    guiOutput.loadFromFile("settingsOutput.xml");
    guiOutput.add( columDrawingDelay.setup("micros btwen colum ", 100 , 1 , 4000 ) );
    columDrawingDelay.addListener(this, &ofApp::changedColumDrawingDelay );
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
    
    if( retina )
        rx = 2;
    else
        rx = 1;
    
    cout << " setting capture mode to  " <<  captureType << "\n";
    resetSizes();
}
//--------------------------------------------------------------
void ofApp::changedColumDrawingDelay( int & columDrawingDelayRate ){
    microsecondsBetwenLineUpdate = columDrawingDelayRate;
    threadLedSender.setOutputColumDelay( microsecondsBetwenLineUpdate );
    
}
//--------------------------------------------------------------
void ofApp::setInputModeImage(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a image ");
    if (openFileResult.bSuccess){
        ofLogVerbose("User selected a file");
        processOpenFileImage( openFileResult );
    }
    else
        ofLogVerbose("User hit cancel");
}
//--------------------------------------------------------------
void ofApp::setInputModeVideo(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a video file");
    if (openFileResult.bSuccess){
        ofLogVerbose("User selected a file");
        processOpenFileVideo( openFileResult );
    }
    else
        ofLogVerbose("User hit cancel");
}
//--------------------------------------------------------------
void ofApp::setInputModeCamera(){
}
//--------------------------------------------------------------
void ofApp::setInputModeScreen(){
}
//--------------------------------------------------------------
void ofApp::processOpenFileImage(ofFileDialogResult openFileResult){
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    ofFile file (openFileResult.getPath());
    if (file.exists()){
        ofLogVerbose("The file exists - now checking the type via file extension");
        string fileExtension = ofToUpper(file.getExtension());
        if (fileExtension == "JPG" || fileExtension == "PNG" || fileExtension == "jpeg" || fileExtension == "tga") {
            setCaptureType( CAPTURE_FROM_IMAGE_FILE , openFileResult.getPath() );
        }
    }
}
//--------------------------------------------------------------
void ofApp::processOpenFileVideo(ofFileDialogResult openFileResult){
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    ofFile file (openFileResult.getPath());
    if (file.exists()){
        ofLogVerbose("The file exists - now checking the type via file extension");
        string fileExtension = ofToUpper(file.getExtension());
        if (fileExtension == "mpeg" || fileExtension == "mov" || fileExtension == "avi" ) {
            setCaptureType( CAPTURE_FROM_VIDEO_FILE , openFileResult.getPath() );
        }
    }

}
//--------------------------------------------------------------
void ofApp::resetSizes(){
    int captureRestWidth;
    int captureRestHeight;
    float proportion;
    
    switch(captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            proportion = float(imageFromFile.width) / float(imageFromFile.height);
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = imageFromFile.width % outputWidth;
            captureRestHeight = imageFromFile.height % outputHeight;
            
            captureWidth = imageFromFile.width - captureRestWidth;
            captureHeight = imageFromFile.height - captureRestHeight;
            
            numPixelsCapturePerOutputWidth = captureWidth / outputWidth ;
            numPixelsCapturePerOutputHeight = captureHeight / outputHeight;
            
            inputImageResult.clear();
            inputImageResult.allocate( captureWidth , captureHeight , OF_IMAGE_COLOR_ALPHA );
            break;
            
        case CAPTURE_FROM_SCREEN:
            proportion = selector.selectionSize.x / selector.selectionSize.y;
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = int(selector.selectionSize.x) % outputWidth;
            captureRestHeight = int(selector.selectionSize.y) % outputHeight;
            
            captureWidth = selector.selectionSize.x - captureRestWidth;
            captureHeight = selector.selectionSize.y - captureRestHeight;
            
            numPixelsCapturePerOutputWidth = ( rx * captureWidth ) / outputWidth ;
            numPixelsCapturePerOutputHeight = ( rx * captureHeight )  / outputHeight;
            
            inputImageResult.clear();
            inputImageResult.allocate( rx * captureWidth ,  rx * captureHeight , OF_IMAGE_COLOR_ALPHA ) ;
            
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            proportion = float( video.width ) / float( video.height );
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = video.width % outputWidth;
            captureRestHeight = video.height % outputHeight;
            
            captureWidth = video.width - captureRestWidth;
            captureHeight = video.height - captureRestHeight;
            
            numPixelsCapturePerOutputWidth = captureWidth / outputWidth ;
            numPixelsCapturePerOutputHeight = captureHeight / outputHeight;
            
            inputImageResult.clear();
            inputImageResult.allocate( captureWidth , captureHeight , OF_IMAGE_COLOR_ALPHA );
            break;
            
        case CAPTURE_FROM_CAMERA:
            proportion = float( camera.width) / float( camera.height );
            outputWidth = proportion * outputHeight;
            
            captureRestWidth = camera.width % outputWidth;
            captureRestHeight = camera.height % outputHeight;
            
            captureWidth = camera.width - captureRestWidth;
            captureHeight = camera.height - captureRestHeight;
            
            numPixelsCapturePerOutputWidth = captureWidth / outputWidth ;
            numPixelsCapturePerOutputHeight = captureHeight / outputHeight;
            
            inputImageResult.clear();
            inputImageResult.allocate( captureWidth , captureHeight , OF_IMAGE_COLOR_ALPHA );
            break;
            
        default:
            break;
    }
    output.clear();
    output.allocate( outputWidth , outputHeight , OF_IMAGE_COLOR );
    ofVec3f position = ofVec3f( 2 , 0 , 0 );
    ofVec3f normal = ofVec3f( -1 , 0 , 0 );
    projectableImage->setup( &output , position , normal );
    
    threadLedSender.setOutputSize( ofPoint( outputWidth , outputHeight ) );
    
    cout    << "Reseting sizes capture mode to  " <<  captureType << "\n"
    << "\tcapture width = " <<  captureWidth << "," << "\n"
    << "\tcapture height = " <<  captureHeight << "\n" << "\n";
}
//--------------------------------------------------------------
void ofApp::update(){
//     unsigned long long delta = ofGetElapsedTimeMicros() - lastUpdateTime;
//    cout << " delta = " << delta << "\n";
    if( captureType == CAPTURE_FROM_SCREEN ){
        updateImageBackground();
        selector.update(ofPoint( ofGetWindowPositionX() , ofGetWindowPositionY() ) );
    }
    updateImageInput();
    updateImageOutput();
    threadLedSender.setOutputImage( &output );
    lastUpdateTime = ofGetElapsedTimeMicros();
    float angleTeta = threadSensorReciver.getAngleAlpha();
    sphere.update( angleTeta );
    //cout << angleTeta << "\n";
}
//--------------------------------------------------------------
void ofApp::updateImageBackground(){
    unsigned char * pixelsBackground = screenBackground.getPixels();
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int retinalFactor = 1;
    if( retina )
        retinalFactor = 2;
    
    pixelsBackground = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , ofGetWidth()  , ofGetHeight() );
    for( int i = 0; i < retinalFactor * retinalFactor * ofGetWidth() * ofGetHeight() ; i++ ){
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
            
            if( isDebugTestX ){
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
            
            else if( isDebugTestY ){
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
            
            else if( isDebugisDebugTestXY ){
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
    ofDrawBitmapString( "Output", ofPoint( x + 5 , y + 2 ) );
}
//--------------------------------------------------------------
void ofApp::drawLastColum( int x , int y ,  int stripeIndex ){
    int width = ofGetHeight() / outputHeight;
    pixelsOutput = output.getPixels();
    for( int i = 0 ; i < outputHeight ; i ++ ){
        ofFill();
        ofSetHexColor( 0x229922 );
        ofRect( x , y + width * i , width , width  );
        
        if( !(i % 10) )
            ofDrawBitmapString(ofToString(i), ofPoint( x + width ,y + width * i) );
        
        //int pixelIndex =  0 + outputWidth * i;
        int pixelIndex =  outputWidth * i;
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
    inputImageResult.draw( x , y , width , height );
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255);
    ofClear(127);
    if( captureType == CAPTURE_FROM_SCREEN )
        screenBackground.draw(0,0, ofGetWidth() , ofGetHeight() );
    drawLastColum( 0 , 0 , 0 );
    if( ofGetElapsedTimeMillis() - lastTimerUserInteracted < 7000 ){
        drawOutput( 550 , 20 , outputWidth , outputHeight );
        drawInputResul( 550 , 100 , inputImageResult.width , inputImageResult.height );
        
        guiInput.draw();
        guiDebug.draw();
        guiOutput.draw();
        
        drawLastColum( 0 , 0 , 0 );
        
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
