#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);

    toggle_parameters.clear();
    shapes.clear();

    sortable_panel.setup("Sortable");

    unsigned int shape_count = 5;

    for(unsigned int i = 0; i < shape_count; i++) {

        //add shapes controlled by toggles
        ofPath *path = createRandomShape();
        shapes.push_back(ofPtr<ofPath>(path));

        //create parameter for toggle
        ofParameter<bool> toggle;
        toggle.set("shape " + ofToString(i),true);
        toggle_parameters.add(toggle);
        //set sortable_panel to create a toggle connected to the parameter
        sortable_panel.add(toggle, true);
        sortable_panel.getControl("shape " + ofToString(i))->setTextColor(path->getFillColor());
    }

    ofAddListener(sortable_panel.elementRemoved, this, &ofApp::removeForm);
    ofAddListener(sortable_panel.elementMovedStepByStep, this, &ofApp::reorderForm);

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
}

//--------------------------------------------------------------
void ofApp::draw(){

    for(int i = shapes.size()-1; i >= 0 ; i--) {
        if(toggle_parameters.getBool(i))
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

//gets called by "sortable_panel" when an item is dragged outside of panel
//deletes the entry in toggle parameters and shapes with the index of the dropped toggle
void ofApp::removeForm(RemovedElementData &data) {

    //keep app parameter list and shape list in sync with sortable_panel
    ofParameterGroup p;
    for(int i = 0; i < toggle_parameters.size(); i++){
        if(i != data.index){
            p.add(toggle_parameters.get(i));
        }
    }
    toggle_parameters = p;
    shapes.erase(shapes.begin()+data.index);

}

//gets called by "sortable_panel" when items get rearranged
//reorders lists of toggle parameters and shapes according when toggles change indices
void ofApp::reorderForm(MovingElementData &data) {
    ofParameterGroup p;
    for(int i = 0; i < toggle_parameters.size(); i++){
        if(i == data.new_index){
            p.add(toggle_parameters.get(data.old_index));
        }else{
            if(i == data.old_index){
                p.add(toggle_parameters.get(data.new_index));
            }else{
                p.add(toggle_parameters.get(i));
            }
        }
    }
    toggle_parameters = p;
    std::swap( shapes[data.old_index], shapes[data.new_index] );
}
