#pragma once
#include "ofMain.h"
#include "ofxGui.h"

struct listitem : public ofPtr<ofxBaseGui> {
    listitem(ofxBaseGui *t):ofPtr<ofxBaseGui>(t){}
    listitem(ofPtr<ofxBaseGui> t):ofPtr<ofxBaseGui>(t) {}
    ofPoint original_position;
};

struct MovingElementData {
    MovingElementData(int i1, int i2, ofPtr<ofxBaseGui> _widget):old_index(i1),new_index(i2),widget(_widget),name(_widget->getName()) {}
    int old_index, new_index;
    string name;
    ofPtr<ofxBaseGui> widget;
};

struct RemovedElementData {
    RemovedElementData(int _index, string _name):index(_index),name(_name) {}
    int index;
    string name;
};

class ofxSortableList : public ofxPanel {

public:
    ofxSortableList();

    void setup(string title = "");

//    void add(const ofParameterGroup & parameters, bool at_end = true);
    void add(ofParameter<bool> & parameter, bool at_end = true);
    void add(ofParameter<string> & parameter, bool at_end = true);

    void add(ofPtr<ofParameter<bool>> parameter, bool at_end = true);
    void add(ofPtr<ofParameter<string>> parameter, bool at_end = true);

    template<class T1, class T2>
    void add(ofPtr< ofParameter<T2>> item, bool at_end = true) {
        listitem guiitem(new T1(*item.get()));
        _add(guiitem, at_end);
    }

    void _add(listitem item, bool at_end);

    void clear();
    void refill();
    void shift(int start_index);

    bool mouseDragged(ofMouseEventArgs& args);
    bool mousePressed(ofMouseEventArgs& args);
    bool mouseReleased(ofMouseEventArgs& args);
    bool mouseMoved(ofMouseEventArgs& args);
    bool mouseScrolled(ofMouseEventArgs& args);

    //is called after an element is dropped outside of the list.
    ofEvent <RemovedElementData> elementRemoved;

    //is called after an element is dropped and changed position.
    ofEvent <MovingElementData> elementMoved;

    //is called for every single index an element is moved. when an element is moved from 2 to 4, it is called two times (2->3, 3->4)
    ofEvent <MovingElementData> elementMovedStepByStep;

    vector<listitem> getListItems();

private:
    void readd(listitem &btn);
    void switchPositions(listitem &t1, listitem &t2);
    vector<listitem> list;
    bool catched_el;
    int moving_el;

};

