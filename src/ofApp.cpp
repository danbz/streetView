#include "ofApp.h"
#include <fstream>

#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    //    viewLat = 50.7530769;//liege netherlands border post
    //    viewLong = 5.6960133;//liege netherlands border post
    
    pie = 3.14159265358979323846;
    viewLat = 51.462088;//stokes croft
    viewLong = -2.5901384;
    
    // viewLat = 22.2040435; // lo shing beach, lamma island, hong kong
    // viewLong = 114.1228474;
    //
    // viewLat = 22.2738285; // tai lung fung
    // viewLong = 114.1742357;
    //
    // viewLat = 22.3290091; // sham sui po
    // viewLong = 114.1600577;
    
    // viewLat = 51.5130679;
    // viewLong =-0.228804; // RCA white city
    
    // viewLat = 50.3621444; //EDEN PROJECT
    // viewLong =-4.7448747;
    
    // 55.5893491,12.6428642 dragor
    // 22.2040435,114.1228474 lo shing beach, lamma island, hong kong
    
    // streetview.setLatLon(40.75732,-73.985951);  // Time Sq
    // streetview.setLatLon(40.768153,-73.981473); // Columbus Circus
    // streetview.setLatLon(40.751511,-73.993953);  // Penn Station
    // streetview.setLatLon(22.276499,114.1735439); // wanchai MTR Hong Kong;
    // streetview.setLatLon( 51.462088,-2.5901384 ); //stokes croft
    // streetview.setLatLon( 50.7530769,5.6964121 ); //liege netherlands border post
    // streetview.setLatLon( 50.7531791,5.6960133 ); //liege netherlands border post  2
    
    ofxStreetView newStreet;
    streetview.push_back(newStreet);
    streetview[0].setLatLon(viewLat, viewLong);
    streetview[0].setZoom(3);
    
    b_drawPointCloud = true;
    b_enableLight = false;
    b_updateMesh=false;
    b_showGui = true;
    linkLevel =0;
    
    fileName = "streetmesh" + ofGetTimestampString() + ".obj";
    cout << fileName << endl;
    
   // obj.open(ofToDataPath(fileName),ofFile::WriteOnly);
    // obj.open(ofToDataPath(fileName),ofFile::ReadWrite);
    mesh = streetview[0].getDethMesh();
    
    gui.setup();
    string num;
    for(int i = 0; i < 20; i++){
        num = std::to_string(i);
        gui.add(showMesh[i].setup("showMesh "+num,1));
        //gui.add(latOffset[i].setup("latOffset"+num, 0, -20, 20));
        //gui.add(longOffset[i].setup("longOffset"+num, 7.0, -20, 20));
        gui.add(rotOffset[i].setup("Rotation offset"+num, 0, -180, 180));
    }

    gui.add(pointSize.setup("pointSize", 2, 1, 10));
    gui.add(scaleMeters.setup("scaleMeters", 20000.0, 1000.0, 500000.0));
    
    // good values for stokes croft first 5 going north from junction with city road
    latOffset[0] = -0;
    longOffset[0] = 0;
    rotOffset[0] = 0;
    
    latOffset[1] = -0.4;
    longOffset[1] = -5.8;
    rotOffset[1] = -37;
    
    latOffset[2] = -0.4;
    longOffset[2] = -3.4;
    rotOffset[2] = 154;
    
    latOffset[3] = 0.200001;
    longOffset[3] = -10.6;
    rotOffset[3] = -97;
    
    latOffset[4] = 0.200001;
    longOffset[4] = 3;
    rotOffset[4] = 91;
    
    latOffset[5] = 0.599998;
    longOffset[5] = 1.6;
    rotOffset[5] = -149;
    
    latOffset[6] = 0;
    longOffset[6] = 8.6;
    rotOffset[6] = 120;
}

//--------------------------------------------------------------
void ofApp::update(){
    // iterate through vector of streetview objects
    for(int i = 0; i < streetview.size(); i++){
        streetview[i].update();
        streetview[i].setUseTexture(true);
    }
    
    //    if (b_updateMesh) {
    //        mesh.append(streetview.getDethMesh());
    //        b_updateMesh= false;
    //    }
    
    }

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    stringstream statusStream, statusStream2;
    
    int mapSize =3000;
    if (b_enableLight) worldLight.enable();
    cam.begin();
    ofDrawAxis(100);
    ofDrawRotationAxes(1);
    
   // double doubleScale = scaleMeters;
    if (b_drawPointCloud) {
        glPointSize(pointSize);
        
        //int homeXOffset = (streetview[0].getLon() * 20037508.34)/180;
        //int homeYOffset = log(tan((90 + streetview[0].getLat()) * (pie/360) ))/ (pie/180) * (20037508.34/180);
        home = ofxGeo::Coordinate(streetview[0].getLat(), streetview[0].getLon());
        

        
            for(int i = 0; i < streetview.size(); i++){
                if ( streetview[i].bDataLoaded & streetview[i].bPanoLoaded) {
                    
                    ofPushMatrix();

                    double latOffset = streetview[0].getLat()- streetview[i].getLat();
                    double longOffset = streetview[0].getLon()- streetview[i].getLon();
                    

                    // cout << latOffset << " lat:long offset " << longOffset << endl;
                    // cout << "mesh " << i << " lat long pixel displacement " << latOffset << "," << longOffset << endl;
                    
                    // cavallo version
                    // Data: Latitute and longitude of a location
                    // Result: [x,z] offset from the center of the scene
                    // int xoffset = (streetview[i].getLon() * 20037508.34)/180 - homeXOffset;
                    // int zoffset = log(tan((90 + streetview[i].getLat()) * (pie/360) ))/ (pie/180) * (20037508.34/180) - homeYOffset;
                    // end cavallo
                    
                    //ofTranslate( - longOffset * scaleMeters /2 , - latOffset * scaleMeters);
                    newLocation = ofxGeo::Coordinate(streetview[i].getLat(), streetview[i].getLon());

                    distanceHaversine = ofxGeo::Utils::distanceHaversine(home, newLocation);
                    bearingHaversine = ofxGeo::Utils::bearingHaversine(home, newLocation);
                    
                    //if  (i > 0 ) ofRotateZ( streetview[i].getDirection()); // rotate to align pano shot direction

                    //ofRotateZ(98); //correct alignment of meshes
                    ofRotateZ(rotOffset[i]);

                    if (showMesh[i]) streetview[i].draw();
                    ofPopMatrix();

                    statusStream2 << "mesh " << i << " dist " << distanceHaversine << "m, bear " << bearingHaversine << ", rot " << home << " " << newLocation <<endl;
                }
        }
        
    } else {
        mesh.setMode(OF_PRIMITIVE_POINTS);
        
        glPointSize(4);
        //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
        ofPushMatrix();
        ofRotateZ(98); //correct alignment of meshes
        // ofScale(1, -1, -1);  // the projected points are 'upside down' and 'backwards'
        // ofTranslate(0, 0, 0); // center the points a bit
        glEnable(GL_DEPTH_TEST);
        //gluPerspective(57.0, 1.5, 0.1, 20000.0); // fov,
        glShadeModel(GL_TRIANGLES);
        
        //mesh.drawVertices();
        mesh.drawFaces();
        ofSetColor( 255, 255, 255);  //set render colour for unpainted points, faces and lines
        mesh.draw();
        glDisable(GL_DEPTH_TEST);
        // mesh.clear();
        ofPopMatrix();
    }
    cam.end();
    worldLight.disable();
    
    ofSetColor(255, 255, 255);
    
    statusStream << streetview[0].getPanoId() << " lat " << viewLat << " lon " << viewLong << " dir " << streetview[0].getDirection() << streetview[0].getAddress() << ", " << streetview[0].getRegion() << "meshes " <<streetview.size() << " linkLvl " << linkLevel;
    if (b_showGui) {
        ofDrawBitmapString(statusStream.str(), 20,  20);
        ofDrawBitmapString(statusStream2.str(), 20,  40);
        gui.draw();
    }
}

//--------



void ofApp::loadLinks(){
    //load next level of related links from the central start streetview
    int numOfLinks, i, n, a;
    string newPanoName;
    bool b_panoExists=false;
    
    
    i = 0;
    int loopStart = linkLevel;
    int loopEnd = streetview.size();
    for (a = loopStart; a < loopEnd; a++) { // loop though non link-read panoramas
        numOfLinks = streetview[linkLevel].links.size(); //get number of links related to a loaded panormama
        cout << numOfLinks << " number of links related to streetview Pano number " << linkLevel << endl;
        if (numOfLinks>0) {
            for (n=0; n<numOfLinks; n++) { //iterate through links adding them
                newPanoName = streetview[linkLevel].links[n].pano_id;
                b_panoExists = false;
                cout << newPanoName << " = new pano name, number " << n+1 << " of " << numOfLinks << endl;
                while (i < streetview.size()) { // check if pano already loaded
                    if (newPanoName == streetview[i].getPanoId()) {
                        b_panoExists = true;
                        break;
                    }
                    i++;
                }
                if (!b_panoExists){
                    cout << " adding pano " << newPanoName << endl;
                    ofxStreetView newStreet;
                    streetview.push_back(newStreet);
                    streetview[i].setPanoId(newPanoName);
                    streetview[i].setZoom(3);
                    b_updateMesh=true;
                } else {
                    cout << "pano " <<  newPanoName << " is already loaded" << endl;
                }
            }
        }
    }
    linkLevel ++;
}

//-----------
void ofApp::calculateVector() {
    
    // geo calc need to add into vector creation in loading new meshes...
    // plus add in new code for mesh transforms and appending on export...
    
    home = ofxGeo::Coordinate(streetview[0].getLat(), streetview[0].getLon());
    
    int i =  streetview.size()-1;
    
    newLocation = ofxGeo::Coordinate(streetview[i].getLat(), streetview[i].getLon());
    
    distanceSpherical = ofxGeo::Utils::distanceSpherical(home, newLocation);
    distanceHaversine = ofxGeo::Utils::distanceHaversine(home, newLocation);
    bearingHaversine = ofxGeo::Utils::bearingHaversine(home, newLocation);
    midpoint = ofxGeo::Utils::midpoint(home, newLocation);
    string closeLink = streetview[i].getCloseLinkTo(0) ;
    string thisLink = streetview[0].getPanoId();
    cout <<  "home to mesh " << i << " distanceHaversine: " << distanceHaversine << " bearing: " << bearingHaversine << " this link: " << thisLink << " closest link: "<< closeLink << endl;
}

//--------------------------------------------------------------

void ofApp::loadNewStreet(int direction){
    string newPanoName;
    int i;
    ofxStreetView newStreet;
    newPanoName = streetview[i-1].getCloseLinkTo(direction);
    
    while (i < streetview.size()) {
        if (newPanoName == streetview[i].getPanoId()) {
            cout << "pano is already loaded" << endl;
            ofSystemAlertDialog("pano is already loaded");
            return;
        }
        i++;
    }
    streetview.push_back(newStreet);
    i = streetview.size()-1;
    streetview[i].setPanoId(newPanoName);
    streetview[i].setZoom(3);
    b_updateMesh=true;
}

//--------------------------------------------------------------

void ofApp::exportOBJ(ofMesh &mesh){
    
    mesh.clear();
    for(int i = 0; i < streetview.size(); i++){ //build new mesh to export
        
       // ofRotateZ(streetview[i].getDirection()+rotOffset[i]);
       // ofTranslate(streetview[i].getLon()*longOffset[i], streetview[i].getLat()*latOffset[i], 0);
        streetview[i].getTexture().bind();
        mesh.append(streetview[i].getDethMesh());
        streetview[i].getTexture().unbind();
    }
    
    //obj.open(ofToDataPath(name),ofFile::WriteOnly);
    obj << "#vertices\n";
    
    for(int i = 0 ; i < mesh.getNumVertices(); i++) {
        ofVec3f v = mesh.getVertex(i);
        obj << "v " + ofToString(v.x) + " " + ofToString(v.y) + " " + ofToString(v.z) + "\n";
    }
    obj << "#faces\n";
    for(int i = 0 ; i < mesh.getNumIndices(); i += 3)
        obj << "f " + ofToString(mesh.getIndex(i)) + " " + ofToString(mesh.getIndex(i+1)) + " " + ofToString(mesh.getIndex(i+2)) + "\n";
    obj << "\n";
    obj.close();
    cout << "wrote obj file"  << endl;
}

//---------------------------------------------------------------

void ofApp::exportPLY( ofMesh &mesh){
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".ply", "Export your mesh as a.ply file");
    if (saveFileResult.bSuccess){
        //volcaMeshMaker.makeMesh(filteredDepthImage, filteredColorImage, mesh, volca, volcaRenderer);
        
        mesh = streetview[0].getDethMesh();
        
        // color the depth map from the pano texture
        // ofTexture texture = panoFbo.getTexture();
        // ofPixels pixels;
        // texture.readToPixels(pixels);
        // c = pixels.getColor(x,y);
        // meshDepth.addColor(c);
        
        mesh.save(saveFileResult.filePath);
    }

}

//--------------------------------------------------------------

void ofApp::saveCacheFiles(){
    ofBuffer cacheBuffer;
    string panoId;
    
    ofSetDataPathRoot("../../../data/");
    
    for(int i=0; i< streetview.size(); i++){
        panoId = streetview[i].getPanoId();
        fileName = "smCache_" + panoId + ".data";
        
        ofstream output_file(fileName, ios::binary);
        //output_file.write("hello", 100);
        output_file.write((char *)&streetview[i], sizeof(streetview[i]));
        output_file.close();
        
        //obj.open(ofToDataPath(fileName),ofFile::WriteOnly);
        // cacheBuffer.set((char*)streetview, sizeof(streetview));
        // cacheBuffer.set(streetview[1], sizeof(streetview[i]);
        // obj.writeFromBuffer(cacheBuffer);
        //obj.close();
        
        cout << "write cache file" << endl;
    }
}

//--------------------------------------------------------------

void ofApp::loadOBJ(ofMesh &mesh){
    
    //    //obj.open(ofToDataPath(name),ofFile::WriteOnly);
    //    obj << "#vertices\n";
    //
    //    for(int i = 0 ; i < mesh.getNumVertices(); i++) {
    //        ofVec3f v = mesh.getVertex(i);
    //        obj << "v " + ofToString(v.x) + " " + ofToString(v.y) + " " + ofToString(v.z) + "\n";
    //    }
    //    obj << "#faces\n";
    //    for(int i = 0 ; i < mesh.getNumIndices(); i += 3)
    //        obj << "f " + ofToString(mesh.getIndex(i)) + " " + ofToString(mesh.getIndex(i+1)) + " " + ofToString(mesh.getIndex(i+2)) + "\n";
    //    obj << "\n";
    //    obj.close();
    cout << "read obj file"  << endl;
}

//--------------------------------------------------------------

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    
    ofFile file (openFileResult.getPath());
    
    if (file.exists()){
        //Limiting this example to one image so we delete previous ones
        //        processedImages.clear();
        //        loadedImages.clear();
        
        ofLogVerbose("The file exists - now checking the type via file extension");
        string fileExtension = ofToUpper(file.getExtension());
        
        //We only want images
        if (fileExtension == "obj" || fileExtension == "OBJ") {
            
            cout << "its an obj file ok" << endl;
            //Save the file extension to use when we save out
            //originalFileExtension = fileExtension;
            
            //Load the selected image
            //            ofImage image;
            //            image.load(openFileResult.getPath());
            //            if (image.getWidth()>ofGetWidth() || image.getHeight() > ofGetHeight())
            //            {
            //                image.resize(image.getWidth()/2, image.getHeight()/2);
            //            }
            //loadedImages.push_back(image);
            
            //Make some short variables
            //            int w = image.getWidth();
            //            int h = image.getHeight();
            
            //Make a new image to save manipulation by copying the source
            //            ofImage processedImage = image;
            
            //Walk through the pixels
            //            for (int y = 0; y < h; y++){
            //
            //                //Create a vector to store and sort the colors
            //                vector<ofColor> colorsToSort;
            //
            //                for (int x = 0; x < w; x++){
            //
            //                    //Capture the colors for the row
            //                    ofColor color = image.getColor(x, y);
            //                    colorsToSort.push_back(color);
            //                }
            //
            //                //Sort the colors for the row
            //               // sort (colorsToSort.begin(), colorsToSort.end(), sortColorFunction);
            //
            //                for (int x = 0; x < w; x++)
            //                {
            //                    //Put the sorted colors back in the new image
            //                    processedImage.setColor(x, y, colorsToSort[x]);
            //                }
            //            }
            //Store the processed image
            // processedImages.push_back(processedImage);
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    
    switch (key) {
            
        case 'p':
        case 'P':
            b_drawPointCloud =!b_drawPointCloud;
            break;
            
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
            
        case 'l':
        case 'L':
            //b_enableLight = !b_enableLight;
            calculateVector();
            
            break;
            
        case 's':
        case 'S':
            //exportOBJ(mesh);
            saveCacheFiles();
            break;
            
        case 'e':
        case 'E':
            exportPLY(mesh);
            break;
            
        case 'g':
        case 'G':
            b_showGui=!b_showGui;
            break;
            
        case 'o':
        case 'O':
            //Open the Open File Dialog
            openFileResult= ofSystemLoadDialog("Select an obj file");
            //Check if the user opened a file
            if (openFileResult.bSuccess){
                ofLogVerbose("User selected a file");
                //We have a file, check it and process it
                processOpenFileSelection(openFileResult);
            }
            else {
                ofLogVerbose("User hit cancel");
            }
            break;
            
        case OF_KEY_UP:
            loadNewStreet(0);
            break;
            
        case OF_KEY_DOWN:
            loadNewStreet(180);
            break;
            
        case OF_KEY_LEFT:
            loadNewStreet(90);
            break;
            
        case OF_KEY_RIGHT:
            loadNewStreet(270);
            break;
            
        case '+':
        case '=':
            loadLinks();
            break;
            
    }
}

//--------------------------------------------------------------

void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
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
