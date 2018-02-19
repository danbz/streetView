#pragma once

#include "ofMain.h"
#include "ofxStreetView.h"
#include "ofxGui.h"
#include "ofxGeo.h"



class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
		
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);    
    
    //db hack nov 2017
    void exportOBJ(ofMesh &mesh);
    void exportPLY(ofMesh &mesh);

    void loadOBJ(ofMesh &mesh);
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void calculateVector();
    void loadNewStreet( int degrees);
    void loadLinks();
    void saveCacheFiles();
    //gui
    ofxFloatSlider latOffset[20], longOffset[20], scaleMeters;
    ofxIntSlider rotOffset[20], pointSize;
    ofxToggle filled;
    ofxToggle showMesh[20];
    ofxPanel gui;
    
    ofxGeo::Coordinate home;
    ofxGeo::Coordinate newLocation;
    ofLight worldLight;
    ofMesh mesh;
    ofFile obj;
    ofEasyCam cam;
    
    vector <ofxStreetView> streetview;
    ofFileDialogResult openFileResult;
    
    double distanceSpherical = 0;
    double distanceHaversine = 0;
    double bearingHaversine = 0;
    double viewLat, viewLong;
    bool b_drawPointCloud, b_enableLight, b_updateMesh, b_showGui;;
    string fileName;
    int linkLevel;
    double pie;
    ofxGeo::Coordinate midpoint;
    
};
