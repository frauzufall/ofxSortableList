#include "ofxSortableList.h"

ofxSortableList::ofxSortableList(const std::string & name, const ofJson& config):ofxGuiGroup(name, config) {

	dummy = nullptr;
	clear();
	registerMouseEvents();

}

ofxSortableList::~ofxSortableList(){
	unregisterMouseEvents();
}

//void ofxSortableList::_add(ofxGuiElement* item, bool at_end) {
//	//create and insert new element in list

//	ofxGuiGroup::add(item);
//	int index = children().size()-1;
//	if(!at_end){
//		for(int i = index; i > 0; i--){
//			swap(i,i-1);
//		}
//		index = 0;
//	}

//	sizeChangedCB();

//}

void ofxSortableList::clear() {
	draggingElement = false;
	draggedElement = nullptr;
	ofxGuiGroup::clear();
}

bool ofxSortableList::mousePressed(ofMouseEventArgs &args) {

	if(ofxGuiGroup::mousePressed(args)){
		return true;
	}

	for(unsigned int i = 0; i < getControls().size(); i++) {
		if(getControl(i)->isMouseOver()) {
			// mouse pressed on element
			draggedElement = getControl(i);
			draggingElement = true;
			draggedElementOldPos = draggedElement->getPosition();
			mouseOffset = draggedElementOldPos - screenToLocal(args);
			draggedElement->setLayoutPosition(DOM::LayoutPosition::ABSOLUTE);
			draggedElement->setPosition(screenToLocal(args) + mouseOffset);
			dummy = add<ofxGuiElement>();
			//inserts dummy after dragged control as a placeholder
			dummy->setShape(draggedElement->getShape());
			moveChildToIndex(dummy, getControlIndex(draggedElement)+1);
			moveChildToFront(draggedElement);
			invalidateChildShape();
		}
	}

	return false;

}

bool ofxSortableList::mouseDragged(ofMouseEventArgs &args) {

	if(ofxGuiGroup::mouseDragged(args)){
		return true;
	}

	if(draggingElement && draggedElement) {

		//move el with drag
		draggedElement->setPosition(screenToLocal(args) + mouseOffset);

		int draggedElementIndex = getControlIndex(dummy);

		if(isMouseOver()) {
			int new_pos = 1;
			for(int i = 0; i < (int)getControls().size(); i++) {
				ofxGuiElement* element = getControl(i);
				if(element != draggedElement && element != dummy){
					if(element->getPosition().y < screenToLocal(args).y) {
						if(draggedElementIndex < i) {
							new_pos = i;
						}
						if(draggedElementIndex > i) {
							new_pos = i+1;
						}
					}
				}
			}
			if(draggedElementIndex != new_pos) {

				int old_index = draggedElementIndex;
				int new_index = new_pos;

				if(old_index != new_index) {
					if(old_index < new_index) {
						for(int i = old_index; i < new_index; i++) {
//							moving_el_old_pos = getControl(i)->getPosition();
							swap(i, i+1);
							MovingElementData data(i-1, i, getControl(i+1));
							ofNotifyEvent(elementMovedStepByStep, data);
						}
					}
					else {
						for(int i = old_index; i > new_index; i--) {
//							moving_el_old_pos = getControl(i-1)->getPosition();
							swap(i, i-1);
							MovingElementData data(i-1, i-2, getControl(i-1));
							ofNotifyEvent(elementMovedStepByStep, data);
						}
					}
				}

				MovingElementData data(old_index-1, new_index-1, getControl(new_index));
				ofNotifyEvent(elementMoved, data);

			}
		}

		return true;

	}

	return false;
}

bool ofxSortableList::mouseReleased(ofMouseEventArgs &args){

	if(ofxGuiGroup::mouseReleased(args)){
		return true;
	}

	if(draggingElement) {

		if(!isMouseOver()) {
			//remove element
			RemovedElementData data(getControlIndex(dummy)-1, draggedElement->getName());
			removeChild(draggedElement);
//			ofParameterGroup p;
//			for(int i = 0; i < (int)parameters.size(); i++){
//				if(i != moving_el){
//					p.add(parameters.get(i));
//				}
//			}
//			parameters.clear();
//			parameters = p;
			ofNotifyEvent(elementRemoved, data);
		}
		else {
			//reorder elements
			draggedElement->setLayoutPosition(DOM::LayoutPosition::STATIC);
			moveChildToIndex(draggedElement, getControlIndex(dummy));
		}
		removeChild(dummy);
		invalidateChildShape();

	}

	draggingElement = false;
	draggedElement = nullptr;
	mouseOffset = ofPoint(0,0);

	return false;

}

void ofxSortableList::swap(int index1, int index2){
	moveChildToIndex((Element*)getControl(index1), index2+1);
	invalidateChildShape();
}
