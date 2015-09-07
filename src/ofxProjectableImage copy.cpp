#include "ofxProjectableImage.h"
//--------------------------------------------------------------
ofxProjectableImage::ofxProjectableImage(){
    image = new ofImage();
}
//--------------------------------------------------------------
void ofxProjectableImage::setup( ofImage* theImageToProject, ofVec3f thePosition , ofVec3f theNormal  ){
    image = theImageToProject;
    imageCenterPosition = thePosition;
    imageNormal =  theNormal;
    imageNormal.normalize();
    rayPointA = ofVec3f(0,0,0);//sphere is in the origin
    planePoint0 = imageCenterPosition;
    planePoint2 = ofVec3f( 0 , 0 , float(image->height)/2.0f );
    imageDirectionV = planePoint2 - planePoint0;
    imageDirectionV.normalize();
    imageDirectionU = imageDirectionV.cross(imageNormal);
    imageDirectionU.normalize();
    planePoint1 = planePoint0 + imageDirectionU * float(image->height)/2.0f;
    distancePlaneOrigen = rayPointA - planePoint0;
    
    matrixTransform.b = planePoint1.x - planePoint0.x;
    matrixTransform.e = planePoint1.y - planePoint0.y;
    matrixTransform.h = planePoint1.z - planePoint0.z;
    
    matrixTransform.c = planePoint2.x - planePoint0.x;
    matrixTransform.f = planePoint2.y - planePoint0.y;
    matrixTransform.i = planePoint2.z - planePoint0.z;
    
    for( int teta = 0 ; teta < 360 ; teta ++ ){
        std::vector< ofPoint > positionsXYForThisTeta;
        for( int fi = 0 ; fi < 180 ; fi ++ ){
            ofPoint thisAnglesXY;
            calculateXYForAngles( teta , fi , thisAnglesXY );
            positionsXYForThisTeta.push_back( thisAnglesXY );
        }
        sphereXY.push_back( positionsXYForThisTeta );
    }
    
    imageDeformed.allocate( 360 , 180 , OF_IMAGE_COLOR );
}
//--------------------------------------------------------------
void ofxProjectableImage::calculateXYForAngles( float angleTeta , float angleFi , ofPoint& thePosInImage ){
    
    
    rayPointB = ofVec3f( cosf( 2 * PI * (angleTeta/360.0) ) , sinf(  2 * PI * (angleTeta/360.0) ) , sinf(  2 * PI * (angleFi/360.0) ) );
    matrixTransform.a = rayPointA.x - rayPointB.x;
    matrixTransform.d = rayPointA.y - rayPointB.y;
    matrixTransform.g = rayPointA.z - rayPointB.z;
    
    thePosInImage = ofPoint(-1,-1);

    if( matrixTransform.determinant() == 0 ){
        return;
    }
    
    matrixTransformInverse = matrixTransform.inverse(matrixTransform);
    parametersIntersection.x =
    matrixTransformInverse.a * distancePlaneOrigen.x +
    matrixTransformInverse.b * distancePlaneOrigen.y +
    matrixTransformInverse.c * distancePlaneOrigen.z;
    
    parametersIntersection.y =
    matrixTransformInverse.d * distancePlaneOrigen.x +
    matrixTransformInverse.e * distancePlaneOrigen.y +
    matrixTransformInverse.f * distancePlaneOrigen.z;
    
    parametersIntersection.z =
    matrixTransformInverse.g * distancePlaneOrigen.x +
    matrixTransformInverse.h * distancePlaneOrigen.y +
    matrixTransformInverse.i * distancePlaneOrigen.z;
    
    ofPoint coordenatesInImage = ofPoint( parametersIntersection.y + float(image->height)/2.0f , parametersIntersection.z + float(image->width)/2.0f );
    
    if( coordenatesInImage.x > image->width || coordenatesInImage.x > image->height )
        return;
    
    thePosInImage = coordenatesInImage;
    int t = 0;
    return;
}
//--------------------------------------------------------------
void ofxProjectableImage::update(){
    for( int teta = 0 ; teta < 360 ; teta ++ ){
        std::vector< ofPoint > positionsXYForThisTeta;
        for( int fi = 0 ; fi < 180 ; fi ++ ){
            ofColor color = ofColor(0,0,0);;
            //getColor( teta , fi , color );
            if( !(sphereXY[teta][fi].x == -1) ){
                color.r = ofMap( sphereXY[teta][fi].x , 0 , image->width , 0 , 255 );
                color.b = ofMap( sphereXY[teta][fi].y , 0 , image->height , 0 , 255 );
            }
            imageDeformed.setColor( teta , fi , color );
        }
    }
    imageDeformed.update();
}
//--------------------------------------------------------------
bool ofxProjectableImage::getColor( float angleTeta , float angleFi , ofColor& theColor ){
    int angleTetaAprox = int( angleTeta );
    int angleFiAprox = int( angleFi );
    ofPoint positionInImage = sphereXY[angleTetaAprox][angleFiAprox];
    if( positionInImage.x == -1 || positionInImage.y == -1 )
        theColor = ofColor(0,0,0);
    else
        theColor = image->getColor(positionInImage.x, positionInImage.x);
}
//--------------------------------------------------------------
void ofxProjectableImage::draw(int x , int y){
    update();
    //image->draw(x,y);
    //imageDeformed.draw( x,y );
}
//--------------------------------------------------------------
void ofxProjectableImage::mouseMoved( int x, int y ){
}
//--------------------------------------------------------------
void ofxProjectableImage::mouseDragged( int x, int y, int button){
}
//--------------------------------------------------------------
void ofxProjectableImage::mousePressed( int x, int y, int button){
}
//--------------------------------------------------------------
void ofxProjectableImage::mouseReleased( int x , int y, int button){
}
//--------------------------------------------------------------
void ofxProjectableImage::windowResized( int w, int h){
}
