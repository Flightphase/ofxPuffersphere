
#include "ofxSimpleGuiSliderRange.h"


ofxSimpleGuiSliderRange::ofxSimpleGuiSliderRange(string name, ofRange& value, float min, float max)
	: ofxSimpleGuiControl(name) 
{
	
	this->min = min;
	this->max = max;
	this->value = &value;	
	controlType = "SliderRange";
	setup();
}

void ofxSimpleGuiSliderRange::setup() {
	setSize(config->gridSize.x - config->padding.x, config->sliderHeight + config->sliderTextHeight);
	range.min = ofMap((*value).min, min, max, x, x+width);
	range.max = ofMap((*value).max, min, max, x, x+width);
}

void ofxSimpleGuiSliderRange::loadFromXML(ofxXmlSettings &XML) {
	value->set(XML.getValue("controls:" + controlType + "_" + key + ":valueMin", 0.0f), 
			   XML.getValue("controls:" + controlType + "_" + key + ":valueMax", 0.0f));
}


void ofxSimpleGuiSliderRange::saveToXML(ofxXmlSettings &XML) {
	XML.addTag(controlType + "_" + key);
	XML.pushTag(controlType + "_" + key);
	XML.addValue("name", name);
	XML.addValue("valueMin", value->min);
	XML.addValue("valueMax", value->max);
	XML.popTag();
}


void ofxSimpleGuiSliderRange::set(float min, float max) {
	value->set(min, max);
}

void ofxSimpleGuiSliderRange::setMin(float min) {
	value->setMin(min);
}

void ofxSimpleGuiSliderRange::setMax(float max) {
	value->setMax(max);
}

void ofxSimpleGuiSliderRange::onPress(int x, int y, int button) {
	lock = true;
	x -= this->x;
	slidingMin = fabs(x - range.min) < fabs(x - range.max);
	if(slidingMin){
		range.setMin(x);
	}
	else{
		range.setMax(x);
	}
}

void ofxSimpleGuiSliderRange::onDragOver(int x, int y, int button) {
	if(lock) {
		x -= this->x;
		if(slidingMin){
			range.setMin(x);
		}
		else{
			range.setMax(x);
		}
	}
}

void ofxSimpleGuiSliderRange::onDragOutside(int x, int y, int button) {
	if(lock) {
		x -= this->x;
		if(slidingMin){
			range.setMin(x);
		}
		else{
			range.setMax(x);
		}
	}
}

void ofxSimpleGuiSliderRange::onRelease() {
	lock = false;
}

void ofxSimpleGuiSliderRange::update() {
//	if(range.min > x + width)			point.x = x + width;
//	else if(point.x < x)				point.x = x;

//	if(point.y > y+height - config->slider2DTextHeight)			point.y = y + height - config->slider2DTextHeight;
//	else if(point.y < y)				point.y = y;
	
	range.min = ofClamp(range.min,		   0, range.max);
	range.max = ofClamp(range.max, range.min, width);
	if(lock){
		value->min = ofMap(range.min, 0, (float)width, min, max);
		value->max = ofMap(range.max, 0, (float)width, min, max);
	}
}

void ofxSimpleGuiSliderRange::draw(float x, float y) {
	setPos(x, y);
	range.set(ofMap((*value).min, min, max, 0, (float)width),
			  ofMap((*value).max, min, max, 0, (float)width));

	
	
	ofEnableAlphaBlending();
	
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	
	setEmptyColor();
	ofRect(0, 0, width, config->sliderHeight);
	
	
	setFullColor();
	ofRect(range.min, 0, range.max-range.min, config->sliderHeight);
	
	setTextBGColor();
	ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
	
	setTextColor();
	ofDrawBitmapString(name+"\n["+ofToString(value->min, 2)+", "+ofToString(value->max, 2)+"]", 3, height+15-config->sliderTextHeight);
	
	glPopMatrix();
	
	/*
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	
	ofFill();
	setFullColor();
	ofRect(0, 0, width, height - config->sliderHeight);
	
	ofFill();
	setTextBGColor();
	ofRect(0, height-config->slider2DTextHeight, width, config->sliderHeight);
	
	
	setTextColor();
//	ofCircle(pointv.x-x, pointv.y-y, 2);
	
	setTextColor();
//	ofLine(pointv.x-x, 0, pointv.x-x, height-config->slider2DTextHeight);
//	ofLine(0, pointv.y-y,width, pointv.y-y);
	
	glPopMatrix();
	*/
	ofDisableAlphaBlending();
	
}