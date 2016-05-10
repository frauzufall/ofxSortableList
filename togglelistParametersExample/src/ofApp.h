#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"
#include "ofxSortableList.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(ofMouseEventArgs &args);
	void mouseDragged(ofMouseEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseReleased(ofMouseEventArgs &args);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:

	ofPath *createRandomShape();

	void removeForm(RemovedElementData& data);
	void reorderForm(MovingElementData& data);

	ofxGui gui;
	ofxSortableList *sortable_panel;

	ofParameterGroup toggle_parameters;
	vector<ofPtr<ofPath>> shapes;

};

