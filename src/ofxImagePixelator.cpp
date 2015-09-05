#include "ofxImagePixelator.h"
//privates
//--------------------------------------------------------------
ofxImagePixelator::ofxImagePixelator(){
    outputImage = 0;
}
//--------------------------------------------------------------
void ofxImagePixelator::setImage( ofImage* theInputImage , int theOutputHeight ,bool theIsRetinal){
    if( !theInputImage ){
        cout << "not seted up pixelator" << "\n";
        return;
    }

    
    inputImage = theInputImage;
    
    proportions = float(inputImage->width) / float(inputImage->height);
    if( !outputImage )
        outputImage = new ofImage();
    outputImage->clear();
    outputImage->allocate( theOutputHeight * proportions , theOutputHeight ,  OF_IMAGE_COLOR );
    
    ofPoint numPixelsInputPerPixelsOutputRest = ofPoint( inputImage->width % outputImage->width, inputImage->height % outputImage->height);
    
    numPixelsInputPerPixelsOutput.x = inputImage->width - numPixelsInputPerPixelsOutputRest.x;
    numPixelsInputPerPixelsOutput.y = inputImage->height - numPixelsInputPerPixelsOutputRest.y;
    
    pixelDensity.x = ( numPixelsInputPerPixelsOutput.x ) / outputImage->width ;
    pixelDensity.y = ( numPixelsInputPerPixelsOutput.y ) / outputImage->height ;
}
//--------------------------------------------------------------
void ofxImagePixelator::update(){
    for( int y = 0 ; y <  outputImage->height  ; y ++){
        for( int x = 0 ; x < outputImage->width  ; x ++){
            ofPoint positioFirstPixel;
            positioFirstPixel.x = x * pixelDensity.x;
            positioFirstPixel.y = y * pixelDensity.y;
            int red = 0 , green = 0 , blue = 0;
            int numPix = 0;
            for( int inputPixelY = positioFirstPixel.y ; inputPixelY < positioFirstPixel.y  + pixelDensity.y ; inputPixelY ++ ){
                for( int inputPixelX = positioFirstPixel.x ; inputPixelX <  positioFirstPixel.x  + pixelDensity.x ; inputPixelX ++ ){
                    ofColor currentPixelColor = inputImage->getColor( inputPixelX , inputPixelY );
                    red     += currentPixelColor.r;
                    green   += currentPixelColor.g;
                    blue    += currentPixelColor.b;
                    numPix++;
                }
            }
            if( numPix == 0 )
                int h = 0;
            red /= numPix;
            green /= numPix;
            blue /= numPix;
            outputImage->setColor( x , y, ofColor( red , green , blue ) );
        }
    }
    outputImage->update();
}
//--------------------------------------------------------------
void ofxImagePixelator::draw(){
    //inputImage->draw( 400 , 50 );
    //outputImage->draw( 200 , 50 );
}
//--------------------------------------------------------------
void ofxImagePixelator::mouseMoved(int x, int y ){
}
//--------------------------------------------------------------
void ofxImagePixelator::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxImagePixelator::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxImagePixelator::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofxImagePixelator::windowResized(int w, int h){
}
//--------------------------------------------------------------
ofPoint ofxImagePixelator::getCapureSize(){
    return numPixelsInputPerPixelsOutput;
}
//--------------------------------------------------------------
unsigned char*  ofxImagePixelator::getOutputPixels(){
    return outputImage->getPixels();
}
//--------------------------------------------------------------
ofImage* ofxImagePixelator::getOutputImage(){
    return outputImage;
}