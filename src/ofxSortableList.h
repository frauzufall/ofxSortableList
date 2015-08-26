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
    ofxSortableList();
    ~ofxSortableList(){}

    void add(ofParameter<bool> & parameter, bool at_end = true);
    void add(ofParameter<std::string> & parameter, bool at_end = true);

    template<class Config>
    void add(ofParameter <bool> & parameter, const Config & config, bool at_end = true);

    template<class Config>
    void add(ofParameter <std::string> & parameter, const Config & config, bool at_end = true);

    template<class GuiType, typename Type, class C>
    void add(ofParameter<Type> p, const C & config = true, bool at_end = true);

    void _add(ofxBaseGui* item, bool at_end);

    void clear();
    void refill();
    void shift(int start_index);

    bool mouseDragged(ofMouseEventArgs& args);
    bool mousePressed(ofMouseEventArgs& args);
    bool mouseReleased(ofMouseEventArgs& args);

    //is called after an element is dropped outside of the list.
    ofEvent <RemovedElementData> elementRemoved;

    //is called after an element is dropped and changed position.
    ofEvent <MovingElementData> elementMoved;

    //is called for every single index an element is moved. when an element is moved from 2 to 4, it is called two times (2->3, 3->4)
    ofEvent <MovingElementData> elementMovedStepByStep;

protected:

    virtual void addOwned(ofxBaseGui * element, bool at_end = true);
    virtual void add(ofxBaseGui * element, bool at_end = true);

private:
    void swap(int index1, int index2);
    bool catched_el;
    int moving_el;
    ofPoint moving_el_old_pos;
    ofVec2f mouse_offset;

};



template<class C>
void ofxSortableList::add(ofParameter <bool> & parameter, const C & config, bool at_end){
    add<ofxToggle>(parameter, config, at_end);
}

template<class C>
void ofxSortableList::add(ofParameter <std::string> & parameter, const C & config, bool at_end){
    add<ofxLabel>(parameter, config, at_end);
}

template<class GuiType, typename Type, class C>
void ofxSortableList::add(ofParameter<Type> p, const C & config, bool at_end){
    auto inContainerConfig = config;
    inContainerConfig.inContainer = true;
    addOwned(new GuiType(p,inContainerConfig), at_end);
}
