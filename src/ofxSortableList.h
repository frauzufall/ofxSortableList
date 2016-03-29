#pragma once
#include "ofMain.h"
#include "ofxGui.h"

struct MovingElementData {
	MovingElementData(int i1, int i2, ofxBaseGui* _widget)
		:old_index(i1),
		  new_index(i2),
		  name(_widget->getName()),
		  widget(_widget) {}
	int old_index, new_index;
	string name;
	ofxBaseGui* widget;
};

struct RemovedElementData {
	RemovedElementData(int _index, string _name):index(_index),name(_name) {}
	int index;
	string name;
};

class ofxSortableList : public ofxGuiGroup {

public:
	ofxSortableList(const string &name="", const ofJson &config = ofJson());
	~ofxSortableList();

//	void clear();
//	void refill();
//	void shift(int start_index);

//	bool mouseDragged(ofMouseEventArgs& args);
//	bool mousePressed(ofMouseEventArgs& args);
//	bool mouseReleased(ofMouseEventArgs& args);

	//is called after an element is dropped outside of the list.
	ofEvent <RemovedElementData> elementRemoved;

	//is called after an element is dropped and changed position.
	ofEvent <MovingElementData> elementMoved;

	//is called for every single index an element is moved. when an element is moved from 2 to 4, it is called two times (2->3, 3->4)
	ofEvent <MovingElementData> elementMovedStepByStep;

protected:

	void onChildAdded(ElementEventArgs& args);
	void onChildMove(MoveEventArgs& args);
	void onChildRemoved(ElementEventArgs& args);


private:
	void swap(int index1, int index2);
	bool catched_el;
	int moving_el;
	ofPoint moving_el_old_pos;
	ofVec2f mouse_offset;

};

