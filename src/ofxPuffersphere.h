/**
 * ofxPuffersphere
 *	
 * Copyright (c) 2011 Flightphase
 *
 * implementaiton by James George (@obviousjim) and Tim Gfrerer (@tgfrerer) for the 
 * Voyagers gallery National Maritime Museum 
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * ofxPuffersphere 
 * Allows you to draw an arbitrary objects onto the puffersphere!
 * Since this screen is spherical, you don't specify a x/y location like normal, instead you specify
 * a rotation from the center to move this object to.
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
    float Z;
    ofVec3f shaderLensCorrection;
} SpherizeShaderParams;


class ofxPuffersphere
{
  public:
	ofxPuffersphere();
	~ofxPuffersphere();

	void setup(float size = 1050); //some are 1050, some are 1080

	#ifdef USE_SIMPLE_GUI
	void addGuiParams(string paramXMLPath = "");
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
	void drawSphere(ofVec3f position, float scale = 100);

	string spherizeLocation;
	string offaxisLocation;
	ofShader* getShaderReference(); //return ref. to the offaxis location

	void addObject(ofxPuffersphereObject* object);
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
