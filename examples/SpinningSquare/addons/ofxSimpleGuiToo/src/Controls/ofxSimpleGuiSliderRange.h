#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofRange.h"

class ofxSimpleGuiSliderRange : public ofxSimpleGuiControl {
public:
	ofRange		*value;
	ofRange		range;
	float		min, max;

	bool slidingMin;
	
	ofxSimpleGuiSliderRange(string name, ofRange& value, float min, float max);
	void setup();
	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	void set(float min, float max);
	void setMin(float min);
	void setMax(float max);
	void onPress(int x, int y, int button);
	void onDragOver(int x, int y, int button);
	void onDragOutside(int x, int y, int button);
	void onRelease();
	void update();
	void draw(float x, float y);
	
};
