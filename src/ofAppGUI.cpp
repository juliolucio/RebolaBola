#include "ofApp.h"
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
}
//--------------------------------------------------------------
void ofApp::setInputModeImage(){
    setCaptureType( CAPTURE_FROM_IMAGE_FILE , "colorGradienY.png" );
//  ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a image ");
//    if (openFileResult.bSuccess){
//        ofLogVerbose("User selected a file");
//        processOpenFileImage( openFileResult );
//    }
//    else
//        ofLogVerbose("User hit cancel");
}
//--------------------------------------------------------------
void ofApp::setInputModeVideo(){
   setCaptureType( CAPTURE_FROM_VIDEO_FILE , "video01.mov" );
//    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a video file");
//    if (openFileResult.bSuccess){
//        ofLogVerbose("User selected a file");
//        processOpenFileVideo( openFileResult );
//    }
//    else
//        ofLogVerbose("User hit cancel");
}
//--------------------------------------------------------------
void ofApp::setInputModeCamera(){
    setCaptureType( CAPTURE_FROM_CAMERA , ":)" );
}
//--------------------------------------------------------------
void ofApp::setInputModeScreen(){
    setCaptureType( CAPTURE_FROM_SCREEN , ":)" );
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
void ofApp::setDrawingStripe01(){
    threadLedSender.setDrawingStripe01( isUpdatingStripe01 );
}
//--------------------------------------------------------------
void ofApp::setDrawingStripe02( ){
    threadLedSender.setDrawingStripe02( isUpdatingStripe02 );
    
}

