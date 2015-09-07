#pragma once
#include "ofMain.h"

class ofxProjectableImage {
private:
    void calculateXYForAngles( float angleTeta , float angleFi , ofPoint& thePosInImage );
public:
    ofxProjectableImage();
    void setup( ofImage* theImageToProject, ofVec3f thePosition , ofVec3f theNormal  );
    void update();
    void draw(int x , int y);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    bool getColor( float angleTeta , float angleFi , ofColor& theColor );
    
    
    ofImage* image;//
    ofImage imageDeformed;//
    ofVec3f imageCenterPosition;//
    ofVec3f imageNormal;//
    ofVec3f imageDirectionU;
    ofVec3f imageDirectionV;
    ofVec3f parametersIntersection;
    ofVec3f distancePlaneOrigen;
    ofVec3f rayPointA;//
    ofVec3f rayPointB;
    ofVec3f planePoint0;//
    ofVec3f planePoint1;//
    ofVec3f planePoint2;//
    
    ofMatrix3x3 matrixTransform;
    ofMatrix3x3 matrixTransformInverse;
    
    std::vector<std::vector< ofPoint > > sphereXY;
    
};
