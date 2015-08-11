#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);

    status = "";

    sortable_labels.setup("Sortable labels");
    sortable_toggles.setup("Sortable toggles");

    uint element_count = 7;

    for(uint i = 0; i < element_count; i++) {

        ofParameter<string> text;
        text.set("label "+ ofToString(i));
        sortable_labels.add(text);

        ofParameter<bool> toggle;
        toggle.set("toggle "+ ofToString(i), true);
        sortable_toggles.add(toggle);

    }

    sortable_toggles.setPosition(sortable_labels.getPosition()+ofVec2f(0,sortable_labels.getHeight()+20));

    ofAddListener(sortable_labels.elementRemoved, this, &ofApp::removed);
    ofAddListener(sortable_labels.elementMoved, this, &ofApp::moved);

    ofAddListener(sortable_toggles.elementRemoved, this, &ofApp::removed);
    ofAddListener(sortable_toggles.elementMoved, this, &ofApp::moved);

}

//--------------------------------------------------------------
void ofApp::exit() {
    ofRemoveListener(sortable_labels.elementRemoved, this, &ofApp::removed);
    ofRemoveListener(sortable_labels.elementMoved, this, &ofApp::moved);

    ofRemoveListener(sortable_toggles.elementRemoved, this, &ofApp::removed);
    ofRemoveListener(sortable_toggles.elementMoved, this, &ofApp::moved);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw(){

    sortable_labels.draw();
    sortable_toggles.draw();

    ofDrawBitmapString(status,400, 20);

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
void ofApp::removed(RemovedElementData& data) {
    status += "removed element with index " + ofToString(data.index) + "\n";
}

//gets called by "sortable_panel" when items get moved
void ofApp::moved(MovingElementData &data) {
    status += "moved element from index " + ofToString(data.old_index) + " to index " + ofToString(data.new_index) + "\n";
}
