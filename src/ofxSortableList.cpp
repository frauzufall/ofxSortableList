#include "ofxSortableList.h"

ofxSortableList::ofxSortableList():ofxPanel() {
}

void ofxSortableList::setup(string title) {

    ofxPanel::setup(title);
    clear();

}

void ofxSortableList::add(ofParameter<string> &parameter, bool at_end) {
    _add(listitem(new ofxLabel(parameter, b.width)), at_end);
}

void ofxSortableList::add(ofParameter<bool> &parameter, bool at_end) {
    _add(listitem(new ofxToggle(parameter, b.width)), at_end);
}

void ofxSortableList::add(ofPtr<ofParameter<bool> > parameter, bool at_end) {
    _add(listitem(new ofxToggle(*parameter.get())), at_end);
}

void ofxSortableList::add(ofPtr<ofParameter<string> > parameter, bool at_end) {
    _add(listitem(new ofxLabel(*parameter.get())), at_end);
}

void ofxSortableList::_add(listitem item, bool at_end) {
    //create and insert new element in list

    ofxPanel::add(item.get());
    int index = 0;
    if(at_end) {
        list.push_back(item);
        index = list.size()-1;
    }
    else {
        list.insert(list.begin(),item);
    }

    //set position of new element
    ofPoint pos = list.at(index)->getPosition();

    list.at(index)->setPosition(pos);
    list.at(index).original_position = pos;

    //move existing elements below new element
    if(!at_end) {
        shift(index);
    }
}

void ofxSortableList::clear() {
    catched_el = false;
    moving_el = -1;
    ofxPanel::clear();
    list.clear();
}

void ofxSortableList::refill() {
    catched_el = false;
    moving_el = -1;
    ofxPanel::clear();
    for(uint i = 0; i < list.size(); i++) {
        readd(list.at(i));
    }
}

void ofxSortableList::readd(listitem &btn) {
    ofxPanel::add(btn.get());
    btn.original_position = btn->getPosition();
}

void ofxSortableList::shift(int start_index) {
    for(int i = list.size()-1; i > start_index; i--) {
        switchPositions(list.at(start_index), list.at(i));
    }
}

bool ofxSortableList::mousePressed(ofMouseEventArgs &args) {

    for(uint i = 0; i < list.size(); i++) {
        list.at(i).original_position = list.at(i)->getPosition();
    }

    for(uint i = 0; i < list.size(); i++) {
        if(list.at(i)->getShape().inside(args.x,args.y)) {
            // mouse pressed on element
            moving_el = i;
            catched_el = true;
            break;
        }
    }

    return ofxPanel::mousePressed(args);

}

bool ofxSortableList::mouseDragged(ofMouseEventArgs &args) {

    if(catched_el && moving_el >= 0 && moving_el < (int)list.size()) {

        //move el with drag
        list.at(moving_el)->setPosition(args.x, args.y);

        if(this->getShape().inside(args.x,args.y)) {
            int new_pos = 0;
            for(int i = 0; i < (int)list.size(); i++) {
                if(list.at(i)->getPosition().y < args.y) {
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
                            switchPositions(list[i], list[i+1]);
                            std::swap( list[i], list[i+1] );
                            MovingElementData data(i, i+1, (ofPtr<ofxBaseGui>)list.at(i+1));
                            ofNotifyEvent(elementMovedStepByStep, data);
                        }
                    }
                    else {
                        for(int i = old_index; i > new_index; i--) {
                            switchPositions(list[i], list[i-1]);
                            std::swap( list[i], list[i-1] );
                            MovingElementData data(i, i-1, (ofPtr<ofxBaseGui>)list.at(i-1));
                            ofNotifyEvent(elementMovedStepByStep, data);
                        }
                    }
                }

                MovingElementData data(old_index, new_index, (ofPtr<ofxBaseGui>)list.at(new_index));
                ofNotifyEvent(elementMoved, data);

                catched_el = true;
                moving_el = new_pos;
            }
        }

    }

    return ofxPanel::mouseDragged(args);
}

bool ofxSortableList::mouseReleased(ofMouseEventArgs &args){

    if(catched_el) {

        if(!this->getShape().inside(args.x,args.y)) {
            //remove element
            RemovedElementData data(moving_el, list.at(moving_el)->getName());
            list.erase(list.begin()+moving_el);
            ofNotifyEvent(elementRemoved, data);
            refill();
        }
        else {
            //reorder elements
            ofPoint pos = list.at(moving_el).original_position;
            list.at(moving_el)->setPosition(pos);
            list.at(moving_el).original_position = pos;
        }

    }

    catched_el = false;
    moving_el = -1;

    return ofxPanel::mouseReleased(args);

}

bool ofxSortableList::mouseMoved(ofMouseEventArgs &args) {
    return ofxPanel::mouseMoved(args);
}

bool ofxSortableList::mouseScrolled(ofMouseEventArgs &args) {
    return ofxPanel::mouseScrolled(args);
}

void ofxSortableList::mouseExited(ofMouseEventArgs &args) {
    return ofxPanel::mouseExited(args);
}

 void ofxSortableList::switchPositions(listitem& t1, listitem& t2) {
     ofPoint p1 = t1.original_position;
     ofPoint p2 = t2.original_position;
     t1->setPosition(p2);
     t1.original_position = p2;
     t2->setPosition(p1);
     t2.original_position = p1;
 }

 vector<listitem>& ofxSortableList::getListItems() {
     return list;
 }
