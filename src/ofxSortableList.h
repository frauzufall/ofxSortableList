#pragma once
#include "ofMain.h"
#include "ofxGui.h"

struct listitem : public ofPtr<ofxBaseGui> {
    listitem(ofxBaseGui *t):ofPtr<ofxBaseGui>(t){}
    listitem(ofPtr<ofxBaseGui> t):ofPtr<ofxBaseGui>(t.get()) {}
    ofPoint original_position;
};

struct MovingElementData {
    MovingElementData(int i1, int i2):old_index(i1),new_index(i2) {}
    int old_index, new_index;
};

class ofxSortableList : public ofxPanel {

public:
    ofxSortableList();

    void setup(string title);
    template<class T>
    void add(ofPtr< ofParameter<bool>> item, bool at_end = true) {

        listitem guiitem(new T(*item.get()));

        //create and insert new element in list

        ofxPanel::add(guiitem.get());
        int index = 0;
        if(at_end) {
            list.push_back(guiitem);
            index = list.size()-1;
        }
        else {
            list.insert(list.begin(),guiitem);
        }

        //set position of new element
        ofPoint pos = list.at(index)->getPosition();

        list.at(index)->setPosition(pos);
        list.at(index).original_position = pos;

        //move existing elements below new element
        if(index != (int)list.size()-1) {
            shift(index+1);
        }

    }
    void add(listitem &btn);
    void clear();
    void refill();
    void shift(int start_index);

    bool mouseDragged(ofMouseEventArgs& args);
    bool mousePressed(ofMouseEventArgs& args);
    bool mouseReleased(ofMouseEventArgs& args);
    bool mouseMoved(ofMouseEventArgs& args);
    bool mouseScrolled(ofMouseEventArgs& args);

    ofEvent <int> elementRemoved;
    ofEvent <MovingElementData> elementMoved;
    vector<listitem> getListItems();

private:
    void switchPositions(listitem &t1, listitem &t2);
    vector<listitem> list;
    bool catched_el;
    int moving_el;

};

