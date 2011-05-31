/*
 *  ofxPuffersphere.h
 *  NMMPuffersphereDesignPrototype
 *
 *  Created by Jim on 5/11/11.
 *  Copyright 2011 FlightPhase. All rights reserved.
 *  
 */

#pragma once

#include "ofxPuffersphereObject.h"

#define USE_SIMPLE_GUI

#ifdef USE_SIMPLE_GUI
#include "ofxSimpleGuiToo.h"
#endif;

typedef struct {
    float fov;
    int projection_method;
    float Z;
} SpherizeShaderParams;


class ofxPuffersphere
{
  public:
	ofxPuffersphere();
	~ofxPuffersphere();
	
	void setup(float size = 1050); //some are 1050, some are 1080
	
	#ifdef USE_SIMPLE_GUI
	void addGuiParams();
	#endif
	
	//updates the current render canvas
	void render();
	
	//if you have elements you want to manually render do begin, then end
	void begin();
	void renderAllObjects(); //just renders the elements, don't call this outside of BEGIN/END
	void end();

	//use this for drawing to the puffersphere itself
	void draw();
	
	//let's you draw the sphere in 3d space to preview what it will look like on the sphere
	void drawSphere(ofVec3f position);
	
	string spherizeLocation;
	string offaxisLocation;
	ofShader* getShaderReference(); //return ref. to the offaxis location
	
	ofxPuffersphereObject* createObject();
	ofxPuffersphereObject* createObject(string imagePath);
	// creates video object. class does not take care of update/play/pause/loop etc.
	ofxPuffersphereObject* createObjectFromVideoPath(string videoPath);
	
	ofFbo * getCanvasRef(){ return &canvas; }
	
	float getSize();
	
  private:
	ofFbo canvas;
	bool shaderLoaded;
	bool hasBegun;
	bool renderForPuffersphere;
	
	float size;
	ofShader offaxis;
	ofShader spherize;
	
	SpherizeShaderParams sphereShaderSettings;
	
	vector<ofxPuffersphereObject*> objects;
};