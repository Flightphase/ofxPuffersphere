/*
 *  ofxCustomGuiResponder.h
 *  NMMVisualizer
 *
 *  Created by Timothy Gfrerer on 17/02/2011.
 *  Copyright 2011. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000


class ofxCustomGuiResponder 
{
public:
 	virtual void objectDidRollOver(void* object, int x, int y) = 0;
	virtual void objectDidPress(void* object, int x, int y, int button) = 0;	
	virtual void objectDidRelease(void* object, int x, int y, int button) = 0;	
};


class ofxCustomGuiElement : public ofxMSAInteractiveObject {
  
public:
	string ISA;
  int id;

  static int id_count;  

	ofxCustomGuiElement(){
    id == id_count++;
		delegate = NULL;
	}
	
	void setup() {
		//printf("MyTestObject::setup() - hello!\n");
		enableMouseEvents();
		enableKeyEvents();
	}	
	
	void setDelegate(ofxCustomGuiResponder* _delegate)
	{
		delegate = _delegate;
		if(delegate == NULL) cout << "Setting a NULL delegate" << endl;
		else cout << "set delegate!" << endl;
	}
	
	void exit() {
		//printf("MyTestObject::exit() - goodbye!\n");
	}
	
	
	void update() {
		//		x = ofGetWidth()/2 + cos(ofGetElapsedTimef() * 0.2) * ofGetWidth()/4;
		//		y = ofGetHeight()/2 + sin(ofGetElapsedTimef() * 0.2) * ofGetHeight()/4;
	}
	
	
	void draw() {
		if(isMouseDown()) ofSetHexColor(DOWN_COLOR);
		else if(isMouseOver()) ofSetHexColor(OVER_COLOR);
		else ofSetHexColor(IDLE_COLOR);
		
		ofRect(x, y, width, height);
	}
	
	virtual void onRollOver(int x, int y) {
		if(delegate != NULL){
			delegate->objectDidRollOver(this, x, y);
		}
		//printf("MyTestObject::onRollOver(x: %i, y: %i)\n", x, y);
	}
	
	virtual void onRollOut() {
		//printf("MyTestObject::onRollOut()\n");
	}
	
	virtual void onMouseMove(int x, int y){
		//printf("MyTestObject::onMouseMove(x: %i, y: %i)\n", x, y);
	}
	
	virtual void onDragOver(int x, int y, int button) {
		//printf("MyTestObject::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onDragOutside(int x, int y, int button) {
		//printf("MyTestObject::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onPress(int x, int y, int button) {
		if(delegate != NULL){
			delegate->objectDidPress(this, x, y, button);
		}
		//printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onRelease(int x, int y, int button) {
		if(delegate != NULL){
			delegate->objectDidRelease(this, x, y, button);
		}
		
		//printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void onReleaseOutside(int x, int y, int button) {
		//printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
	}
	
	virtual void keyPressed(int key) {
		//printf("MyTestObject::keyPressed(key: %i)\n", key);
	}
	
	virtual void keyReleased(int key) {
		//printf("MyTestObject::keyReleased(key: %i)\n", key);
	}
	
private:
	ofxCustomGuiResponder* delegate;
};