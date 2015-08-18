#include "ofApp.h"
extern "C" {
#include "macGlutfix.h"
#include "gamma.h"
}
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    numPixels = 64;
    outputHeight = numPixels;
    
    // setCaptureType( CAPTURE_FROM_IMAGE_FILE , "pano.jpeg" );
    setCaptureType( CAPTURE_FROM_SCREEN , ":)" );
    //setCaptureType( CAPTURE_FROM_VIDEO_FILE , "video01.mov" );
    //setCaptureType( CAPTURE_FROM_CAMERA , ":)" );
    
    updateIndex = 0;
    ofBackground( 0 );
    
    opcClient.setup("127.0.0.1", 7890);
    lastTimeSend = 0;
    
    linesPerSecond.addListener(this, &ofApp::setPixelLineRate );
    
    //Setting the graber f using macbook retina, set to true
    retina = true;
    if( retina  )
        rx = 2;
    else
        rx = 1;
    screenBackground.allocate( rx * ofGetWidth(), rx * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
    
    gui.setup();
    gui.add( linesPerSecond.setup("lines per second ", 400 , 10 , 1000 ) );
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
    return;
    if( updatingAlternating )
        sendFrameToLedsSingle();
    else
        sendFrameToLedsDivided();
}
//--------------------------------------------------------------
void ofApp::updateImageBackground(){
    unsigned char * pixelsBackground = screenBackground.getPixels();
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
    pixelsBackground = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , ofGetWidth()  , ofGetHeight() );
    for (int i = 0; i < rx*rx*ofGetWidth() * ofGetHeight(); i++){
        r = pixelsBackground[ 4 * i + 1 ];
        g = pixelsBackground[ 4 * i + 2 ];
        b = pixelsBackground[ 4 * i + 3 ];
        a = pixelsBackground[ 4 * i + 0 ];
        
        pixelsBackground[ 4 * i + 0 ] = r;
        pixelsBackground[ 4 * i + 1 ] = g;
        pixelsBackground[ 4 * i + 2 ] = b;
        pixelsBackground[ 4 * i + 3 ] = a;
    }
    screenBackground.setFromPixels( pixelsBackground , rx * ofGetWidth(), rx * ofGetHeight() , OF_IMAGE_COLOR_ALPHA );
    screenBackground.update();
}
//--------------------------------------------------------------
void ofApp::updateImageInput(){
    unsigned char * pixelsInput;
    unsigned char * pixelsResult;
    
    int numInputPixels;
    
    pixelsInput = inputImageResult.getPixels();

    
    switch (captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            for( int x = 0 ; x < captureWidth ; x ++ ){
                for( int y = 0 ; y < captureHeight ; y ++ ){
                    inputImageResult.setColor( x , y , imageFromFile.getColor( x, y ) );
                }
            }
            break;
            
        case CAPTURE_FROM_SCREEN:{
            
            //solution :)))
            pixelsInput = pixelsBelowWindow( ofGetWindowPositionX() + selector.position.x , ofGetWindowPositionY() + selector.position.y , captureWidth , captureHeight );
            
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
    
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
            
            

            
            //            pixelsInput = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , captureWidth  , captureHeight );
            //            pixelsInput = pixelsBelowWindow( 0 , 0 , captureWidth  , captureHeight );
            //
            //            for( int indexPixelCapture = 0; indexPixelCapture < captureWidth * captureHeight ; indexPixelCapture++ ){
            //
            //                unsigned char g = pixelsInput[ indexPixelCapture * 4 + 2 ];
            //                unsigned char r = pixelsInput[ indexPixelCapture * 4 + 1 ];
            //                unsigned char b = pixelsInput[ indexPixelCapture * 4 + 3 ];
            //                //unsigned char a = pixelsInput[ indexPixelCapture *4 8 + 0 ];
            //
            //                pixelsResult[ indexPixelCapture * 3 + 0 ] = r;
            //                pixelsResult[ indexPixelCapture * 3 + 1 ] = g;
            //                pixelsResult[ indexPixelCapture * 3 + 2 ] = b;
            //            }
            
            
            //            //first aproach -------------------------------------------------------------------------
            //
            //            cout    << "Updating output imageto  " <<  captureType << "\n"
            //            << "\tcapture width = " <<  captureWidth << "," << "\n"
            //            << "\tcapture height = " <<  captureHeight << "\n" << "\n"
            //            << "\ttotal pixelsx = " <<  rx * rx * captureWidth * captureWidth << "\n" << "\n";
            //
            //
            //            pixelsInput = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , captureWidth  , captureHeight );
            //            for( int indexPixelCapture = 0 ; indexPixelCapture < captureWidth * captureWidth ; indexPixelCapture+= 4 ){
            //                unsigned char g = 0;
            //                unsigned char r = 0;
            //                unsigned char b = 0;
            //                unsigned char a = 0;
            //                for( int indexRetinalPixel = 0 ; indexRetinalPixel < rx * rx ; indexRetinalPixel ++ ){
            //                    int indexPixelRetinalGlobal = indexPixelCapture + indexRetinalPixel;
            //                    g += pixelsInput[ indexPixelRetinalGlobal * 4 + 2 ];
            //                    r += pixelsInput[ indexPixelRetinalGlobal * 4 + 1 ];
            //                    b += pixelsInput[ indexPixelRetinalGlobal * 4 + 3 ];
            //                    a += pixelsInput[ indexPixelRetinalGlobal * 4 + 0 ];
            //                }
            ////                r = int(r)/(rx*rx);
            ////                g = int(g)/(rx*rx);
            ////                b = int(b)/(rx*rx);
            ////                a = int(a)/(rx*rx);
            ////
            //                pixelsResult[ indexPixelCapture * 4 + 0 ] = r;
            //                pixelsResult[ indexPixelCapture * 4 + 1 ] = g;
            //                pixelsResult[ indexPixelCapture * 4 + 2 ] = b;
            //                pixelsResult[ indexPixelCapture * 4 + 3 ] = 255;
            //
            ////                cout    << "Pixel[  " <<  indexPixelCapture << " ]"
            ////                << "\tr = " <<  r << ","
            ////                << "\tg = " <<  g << ","
            ////                << "\tb = " <<  b << "\n";
            //                //---------------------------------------------------------------------------------------
            
            
            // -------------------------------------------------------------------------
            //secong aproach -------------------------------------------------------------------------
            
//            cout    << "Updating output imageto  " <<  captureType << "\n"
//            << "\tcapture width = " <<  captureWidth << "," << "\n"
//            << "\tcapture height = " <<  captureHeight << "\n" << "\n"
//            << "\ttotal pixelsx = " <<  rx * rx * captureWidth * captureWidth << "\n" << "\n";
//            
//            int componentsPerPixell = 4;
//            int componentsPerPixellRetina = componentsPerPixell * rx * rx;
//            
//            pixelsInput = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , captureWidth  , captureHeight );
//            
//            //we order the colors first
//            for (int i = 0; i < captureWidth * captureHeight ; i++){
//                unsigned char r1 = pixelsInput[i*4];
//                pixelsInput[i*componentsPerPixellRetina]   = pixelsInput[i*componentsPerPixellRetina+1];
//                pixelsInput[i*componentsPerPixellRetina+1] = pixelsInput[i*componentsPerPixellRetina+2];
//                pixelsInput[i*componentsPerPixellRetina+2] = pixelsInput[i*componentsPerPixellRetina+3];
//                pixelsInput[i*componentsPerPixellRetina+3] = r1;
//            }
//            inputImageResult.setFromPixels( pixelsInput , rx * captureWidth, rx * captureHeight , OF_IMAGE_COLOR_ALPHA );
//            inputImageResult.update();
            
//            for( int indexPixelCapture = 0 ; indexPixelCapture < captureWidth * captureWidth ; indexPixelCapture++){
//                std::vector< unsigned char > components;
//                for( int indexComponetPixell = 0 ; indexComponetPixell < componentsPerPixellRetina ; indexComponetPixell ++ )
//                    components.push_back( pixelsInput[ indexPixelCapture * componentsPerPixellRetina + indexComponetPixell ] );
//                
//                unsigned char r = components[ 0 ];
//                unsigned char g = components[ 1 ];
//                unsigned char b = components[ 2 ];
//                unsigned char a = components[ 3 ];
//                
//                pixelsResult[ indexPixelCapture * componentsPerPixell + 0 ] = r;
//                pixelsResult[ indexPixelCapture * componentsPerPixell + 1 ] = g;
//                pixelsResult[ indexPixelCapture * componentsPerPixell + 2 ] = b;
//                pixelsResult[ indexPixelCapture * componentsPerPixell + 3 ] = a;
//                
//                //                cout    << "Pixel[  " <<  indexPixelCapture << " ]"
//                //                << "\tr = " <<  r << ","
//                //                << "\tg = " <<  g << ","
//                //                << "\tb = " <<  b << "\n";
//            }
            //---------------------------------------------------------------------------------------
            
            //third aproach -------------------------------------------------------------------------
            
//            cout    << "Updating output imageto  " <<  captureType << "\n"
//            << "\tcapture width = " <<  captureWidth << "," << "\n"
//            << "\tcapture height = " <<  captureHeight << "\n" << "\n"
//            << "\ttotal pixelsx = " <<  rx * rx * captureWidth * captureWidth << "\n" << "\n";
//            
//            int componentsPerPixell = 4;
//            int componentsPerPixellRetina = componentsPerPixell * rx * rx;
//            
//            pixelsInput = pixelsBelowWindow( ofGetWindowPositionX() , ofGetWindowPositionY() , captureWidth  , captureHeight );
//            for( int indexPixelCapture = 0 ; indexPixelCapture < captureWidth * captureWidth ; indexPixelCapture++){
//                
//                unsigned char g = pixelsInput[ indexPixelCapture * 4 + 2 ];
//                unsigned char r = pixelsInput[ indexPixelCapture * 4 + 1 ];
//                unsigned char b = pixelsInput[ indexPixelCapture * 4 + 3 ];
//                //unsigned char a = pixelsInput[ indexPixelCapture *4 8 + 0 ];
//                
//                pixelsResult[ indexPixelCapture * 3 + 0 ] = r;
//                pixelsResult[ indexPixelCapture * 3 + 1 ] = g;
//                pixelsResult[ indexPixelCapture * 3 + 2 ] = b;
//                
//                
//                //                cout    << "Pixel[  " <<  indexPixelCapture << " ]"
//                //                << "\tr = " <<  r << ","
//                //                << "\tg = " <<  g << ","
//                //                << "\tb = " <<  b << "\n";
//            }
            //---------------------------------------------------------------------------------------
        }
            
            
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            video.update();
            pixelsInput = video.getPixels();
            numInputPixels = video.width * video.height;
            for( int pixelIndex = 0 ; pixelIndex < numInputPixels ; pixelIndex ++ ){
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
            numInputPixels = camera.width * camera.height;
            for( int pixelIndex = 0 ; pixelIndex < numInputPixels ; pixelIndex ++ ){
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
            
            int numPixelx = 0;
            
            for( int pixeCaptureY = positionPixelIniCapture.y ; pixeCaptureY < positionPixelIniCapture.y  + numPixelsCapturePerOutputHeight ; pixeCaptureY ++ ){
                for( int pixeCaptureX = positionPixelIniCapture.x ; pixeCaptureX < positionPixelIniCapture.x  + numPixelsCapturePerOutputWidth ; pixeCaptureX ++){
                    ofColor currentPixelColor = inputImageResult.getColor( pixeCaptureX , pixeCaptureY );
                    red     += currentPixelColor.r;
                    green   += currentPixelColor.g;
                    blue    += currentPixelColor.b;
                    numPixelx++;
                }
            }
            
            red /= numPixels;
            green /= numPixels;
            blue /= numPixels;
            
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
    ofRect( x - borderSize , y - borderSize , output.width + 2 * borderSize , output.height + 2 * borderSize );
    
    ofSetColor( 50 , 50, 90 );
    ofRect( x - borderSize / 2 , y - borderSize / 2 , output.width + 2 * borderSize / 2 , output.height + 2 * borderSize / 2 );
    ofSetColor(255);
    ofNoFill();
    output.draw( x , y , width , height );
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofHideCursor();
    ofSetColor(255);
    screenBackground.draw(0,0, ofGetWidth() , ofGetHeight() );
    
    switch (captureType) {
        case CAPTURE_FROM_IMAGE_FILE:
            imageFromFile.draw(0,0);
            break;
            
        case CAPTURE_FROM_SCREEN:
            ofSetColor(50);
            ofRect( 5, 195,  inputImageResult.width  + 10, inputImageResult.height + 10);
            ofSetColor(255);
            inputImageResult.draw( 10 , 200 , inputImageResult.width , inputImageResult.height );
            break;
            
        case CAPTURE_FROM_VIDEO_FILE:
            video.draw(0,0);
            break;
            
        case CAPTURE_FROM_CAMERA:
            camera.draw( 0,0 );
            break;
            
        default:
            break;
    }
    
    ofNoFill();
    ofSetColor( 0 , 200 , 10 );
    ofRect( selector.position.x, selector.position.y, captureWidth, captureHeight);
    ofRect( selector.position.x, selector.position.y, selector.selectionSize.x, selector.selectionSize.y );
    
    //drawOutput( 0 , 0 , ofGetWidth() , ofGetHeight() );
    if( ofGetElapsedTimeMillis() - lastTimerUserInteracted < 7000 ){
        drawOutput( 500 , 20 , outputWidth , outputHeight );
        gui.draw();
    }
    
    //drawinfg selection area
    selector.draw();
    
    //drawing the mouse cursor
    ofSetHexColor( 0x660FF77 );
    ofNoFill();
    ofCircle( ofGetMouseX() , ofGetMouseY() , 4 );
    ofFill();
    ofSetColor( 240 );
    ofCircle( ofGetMouseX() , ofGetMouseY() , 2 );
    ofSetHexColor( 0x33FF33 );
    
    
}
//--------------------------------------------------------------
void ofApp::sendFrameToLedsDivided(){
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
            opcClient.writeChannel( 1, colorsStrip01 );
            //opcClient.writeChannel( 8, colorsStrip02 );
            int currenTime = ofGetElapsedTimeMicros();
            while( currenTime - lastTimeSend < micrisecondsBetwenLineUpdate )
                currenTime = ofGetElapsedTimeMicros();
            lastTimeSend = currenTime;
            
        }
    }
}
//--------------------------------------------------------------
void ofApp::sendFrameToLedsSingle(){
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
            opcClient.writeChannel( 1, colorsStrip01 );
            opcClient.writeChannel( 8, colorsStrip02 );
            
            int currenTime = ofGetElapsedTimeMicros();
            while( currenTime - lastTimeSend < micrisecondsBetwenLineUpdate )
                currenTime = ofGetElapsedTimeMicros();
            lastTimeSend = currenTime;
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
    resetSizes();
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
