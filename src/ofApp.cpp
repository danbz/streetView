#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    viewLat = 51.462088;
    viewLong = -2.5901384;
    
    //streetview.setLatLon(40.75732,-73.985951);  // Time Sq
   //streetview.setLatLon(40.768153,-73.981473); // Columbus Circus
   // streetview.setLatLon(40.751511,-73.993953);  // Penn Station
    
    //streetview.setLatLon(22.276499,114.1735439); // wanchai MTR hong kong;
    
   // streetview.setLatLon( 51.462088,-2.5901384 ); //stokes croft
   // streetview.setLatLon( 50.7530769,5.6964121 ); //liege netherlands border post
     // streetview.setLatLon( 50.7531791,5.6960133 ); //liege netherlands border post  2
    
    
    streetview.setLatLon(viewLat, viewLong);
    
    streetview.setZoom(3);
    
    b_drawPointCloud, b_enableLight = false;
    
     fileName = "streetmesh" + ofGetTimestampString() + ".obj";
    cout << fileName << endl;

    //obj.open(ofToDataPath(fileName),ofFile::WriteOnly);
    
    obj.open(ofToDataPath(fileName),ofFile::ReadWrite);

   
}

//--------------------------------------------------------------
void ofApp::update(){
    
    streetview.update();
    
    //streetview.setUseTexture(false);
    
    mesh = streetview.getDethMesh();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    if (b_enableLight) worldLight.enable();
    cam.begin();
    
    if (b_drawPointCloud) {
    streetview.draw();
    } else {
    
    // db hack nov 2017
    mesh.setMode(OF_PRIMITIVE_POINTS);

    glPointSize(4);
    //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    ofPushMatrix();
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
    stringstream statusStream;
    statusStream   << "lat: " << viewLat << " long: " << viewLong;
    
    ofDrawBitmapString(statusStream.str(), 20,  20);


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
            b_enableLight = !b_enableLight;
            break;
            
        case 's':
        case 'S':
        exportOBJ(mesh);
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
            viewLong += 0.00010;
            streetview.setLatLon(viewLat, viewLong);
            break;
            
        case OF_KEY_DOWN:
            viewLong -= 0.00010;
             streetview.setLatLon(viewLat, viewLong);
            break;
            
        case OF_KEY_LEFT:
            viewLat -= 0.00010;
            streetview.setLatLon(viewLat, viewLong);
            break;
            
        case OF_KEY_RIGHT:
            viewLat += 0.00010;
            streetview.setLatLon(viewLat, viewLong);
            break;
            
            
    }
    
}

//-----------------
void ofApp::exportOBJ(ofMesh &mesh){

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
