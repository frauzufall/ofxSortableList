#include "ofxSortableList.h"

ofxSortableList::ofxSortableList():ofxPanel() {
}

void ofxSortableList::setup(string title = "") {

    ofxPanel::setup(title);
    clear();

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
        add(list.at(i));
    }
}

void ofxSortableList::add(listitem &btn) {
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
                        }
                    }
                    else {
                        for(int i = old_index; i > new_index; i--) {
                            switchPositions(list[i], list[i-1]);
                            std::swap( list[i], list[i-1] );
                        }
                    }
                }

                MovingElementData data(old_index, new_index);

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
            list.erase(list.begin()+moving_el);
            ofNotifyEvent(elementRemoved, moving_el);
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

 void ofxSortableList::switchPositions(listitem& t1, listitem& t2) {
     ofPoint p1 = t1.original_position;
     ofPoint p2 = t2.original_position;
     t1->setPosition(p2);
     t1.original_position = p2;
     t2->setPosition(p1);
     t2.original_position = p1;
 }

 vector<listitem> ofxSortableList::getListItems() {
     return list;
 }
