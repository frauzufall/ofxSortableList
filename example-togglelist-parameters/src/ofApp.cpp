#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);

    toggle_parameters.clear();
    shapes.clear();

    sortable_panel.setup("Sortable");

    uint shape_count = 5;

    for(uint i = 0; i < shape_count; i++) {

        //create parameter for toggle
        stringstream name;
        name << "shape " << i;
        ofPtr<ofParameter<bool>> t(new ofParameter<bool>(name.str(),true));
        toggle_parameters.push_back(t);

        //set sortable_panel to create a toggle connected to the parameter
        sortable_panel.add<ofxToggle>(t, true);
    }

    for(uint i = 0; i < shape_count; i++) {
        shapes.push_back(ofPtr<ofPath>(createRandomShape()));
    }

    ofAddListener(sortable_panel.elementRemoved, this, &ofApp::removeForm);
    ofAddListener(sortable_panel.elementMoved, this, &ofApp::reorderForm);

}

//--------------------------------------------------------------
ofPath *ofApp::createRandomShape() {
    ofPath *p = new ofPath();
    p->moveTo(ofRandom(ofGetWidth()), 0);
    p->lineTo(ofGetWidth(), ofRandom(ofGetHeight()));
    p->lineTo(ofRandom(ofGetWidth()), ofGetHeight());
    p->lineTo(0, ofRandom(ofGetHeight()));
    p->close();
    ofColor c = ofColor::fromHsb(ofRandom(255), 255,255);
    c.a = 200;
    p->setColor(c);
    return p;
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::update() {

    //update shape visibility according to parameters
    for(uint i = 0; i < toggle_parameters.size(); i++) {

        if(toggle_parameters.at(i)->get()) {

            // toggle is true, show filled shape
            shapes.at(i)->setStrokeWidth(0);
            shapes.at(i)->setFilled(true);

        }
        else {

            // toggle is false, show stroke of shape
            shapes.at(i)->setStrokeWidth(5);
            shapes.at(i)->setFilled(false);

        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    for(uint i = 0; i < shapes.size(); i++) {
        shapes.at(i)->draw();
    }

    sortable_panel.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs &args){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs &args){
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

//gets called by sortabel_panel when an item is dragged outside of panel
void ofApp::removeForm(int &index) {
    //keep app parameter list and shape list in sync with sortable_panel
    toggle_parameters.erase(toggle_parameters.begin()+index);
    shapes.erase(shapes.begin()+index);
}

//gets called by sortabel_panel when items ged reordered
//data.old_index, data.new_index
void ofApp::reorderForm(MovingElementData &data) {
    std::swap( toggle_parameters[data.old_index], toggle_parameters[data.new_index] );
    std::swap( shapes[data.old_index], shapes[data.new_index] );
}
