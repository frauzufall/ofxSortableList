#include "ofxSortableList.h"

ofxSortableList::ofxSortableList():ofxGuiGroup() {
}

void ofxSortableList::setup(string title) {

    ofxGuiGroup::setup(title);
    clear();

}

void ofxSortableList::add(ofParameter<bool> &parameter, bool at_end) {
    add(parameter, ofxToggle::Config(), at_end);
}

void ofxSortableList::add(ofParameter<std::string> &parameter, bool at_end) {
    add(parameter, ofxLabel::Config(), at_end);
}

void ofxSortableList::addOwned(ofxBaseGui * element, bool at_end){
//    collectionOwned.emplace_back(element);
    add(element, at_end);
}

void ofxSortableList::add(ofxBaseGui *element, bool at_end){
    _add(element, at_end);
}

void ofxSortableList::_add(ofxBaseGui* item, bool at_end) {
    //create and insert new element in list

    ofxGuiGroup::add(item);
    int index = collection.size()-1;
    if(!at_end){
        for(int i = index; i > 0; i--){
            swap(i,i-1);
        }
        index = 0;
    }

    sizeChangedCB();

}

void ofxSortableList::clear() {
    catched_el = false;
    moving_el = -1;
    ofxGuiGroup::clear();
}

bool ofxSortableList::mousePressed(ofMouseEventArgs &args) {

    for(uint i = 0; i < collection.size(); i++) {
        if(collection.at(i)->getShape().inside(args.x,args.y)) {
            // mouse pressed on element
            moving_el = i;
            catched_el = true;
            moving_el_old_pos = collection.at(i)->getPosition();
            mouse_offset = moving_el_old_pos - ofPoint(args.x,args.y);
            break;
        }
    }

    return ofxGuiGroup::mousePressed(args);

}

bool ofxSortableList::mouseDragged(ofMouseEventArgs &args) {

    if(catched_el && moving_el >= 0 && moving_el < (int)collection.size()) {

        //move el with drag
        collection.at(moving_el)->setPosition(ofPoint(args.x, args.y) + mouse_offset);

        if(this->getShape().inside(args.x,args.y)) {
            int new_pos = 0;
            for(int i = 0; i < (int)collection.size(); i++) {
                if(collection.at(i)->getPosition().y < args.y) {
                    if(moving_el<i) {
                        new_pos = i;
                    }
                    if(moving_el>i) {
                        new_pos = i+1;
                    }
                }
            }
            if(moving_el != new_pos) {

                int old_index = moving_el;
                int new_index = new_pos;

                if(old_index != new_index) {
                    if(old_index < new_index) {
                        for(int i = old_index; i < new_index; i++) {
                            moving_el_old_pos = getControl(i+1)->getPosition();
                            swap(i, i+1);
                            MovingElementData data(i, i+1, getControl(i+1));
                            ofNotifyEvent(elementMovedStepByStep, data);
                        }
                    }
                    else {
                        for(int i = old_index; i > new_index; i--) {
                            moving_el_old_pos = getControl(i-1)->getPosition();
                            swap(i, i-1);
                            MovingElementData data(i, i-1, getControl(i-1));
                            ofNotifyEvent(elementMovedStepByStep, data);
                        }
                    }
                }

                MovingElementData data(old_index, new_index, getControl(new_index));
                ofNotifyEvent(elementMoved, data);

                catched_el = true;
                moving_el = new_pos;
            }
        }

    }

    return ofxGuiGroup::mouseDragged(args);
}

bool ofxSortableList::mouseReleased(ofMouseEventArgs &args){

    if(catched_el) {

        if(!this->getShape().inside(args.x,args.y)) {
            //remove element
            RemovedElementData data(moving_el, collection.at(moving_el)->getName());
            collection.erase(collection.begin()+moving_el);
            ofParameterGroup p;
            for(int i = 0; i < parameters.size(); i++){
                if(i != moving_el){
                    p.add(parameters.get(i));
                }
            }
            parameters.clear();
            parameters = p;
            ofNotifyEvent(elementRemoved, data);
            sizeChangedCB();
        }
        else {
            //reorder elements
            ofPoint pos = moving_el_old_pos;
            collection.at(moving_el)->setPosition(pos);
        }

    }

    catched_el = false;
    moving_el = -1;
    mouse_offset = ofPoint(0,0);

    return ofxGuiGroup::mouseReleased(args);

}

void ofxSortableList::swap(int index1, int index2){
    if(index1 < (int)collection.size() && index2 < (int)collection.size()){
        std::swap(collection.at(index1), collection.at(index2));
        ofParameterGroup p;
        for(int i = 0; i < parameters.size(); i++){
            if(i == index1){
                p.add(parameters.get(index2));
            }else{
                if(i == index2){
                    p.add(parameters.get(index1));
                }else{
                    p.add(parameters.get(i));
                }
            }
        }
        parameters.clear();
        parameters = p;
        sizeChangedCB();
    }else{
        ofLogWarning("ofxSortableList:swap()", "trying to swap indices " + ofToString(index1) + " and " + ofToString(index2) + " but collection size is " + ofToString(collection.size()));
    }
}
