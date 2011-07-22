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
#include "ofMain.h"

class ofxPuffersphereObject
{
  public:
	ofxPuffersphereObject();
	void makeTransparent();
	
	void setup(float dimenions = 1050);
	void draw();
	
	void setTexture(ofBaseHasTexture* newTexture);
	
	void loadImage(string filename);
	void loadVideo(string filename);

	ofVec2f targetSize;
	ofQuaternion rotation;
	ofBaseHasTexture* image; //can be an ofFbo or ofImage, or any other drawable
	ofShader* offaxis; //please share me between all other sphere objects
    
	ofFloatColor tint; // can be used to fade object out
	ofVec2f scale;
	
  private:
	bool isTransparent();
	bool textureIsOurs; //flagged true if we created the texture, so we know to destroy it
	vector<ofVec2f> canvasCoords;
//	string isA;	// remember the original type of the object.
};
