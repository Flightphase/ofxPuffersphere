/**
 *
 *  ofxPuffersphere 
 *
 *  ofxPuffersphereObject.h
 *  ofxPuffersphereObject -- Allows you to draw an arbitrary object onto the puffersphere!
 *  Since this screen is spherical, you don't specify a x/y location like normal, instead you specify
 *  a rotation from the center to move this object to.
 *
 *
 */

#include "ofxPuffersphere.h"

ofxPuffersphereObject::ofxPuffersphereObject(){
	rotation = ofQuaternion(0,0,0,1);
	tint = ofColor(1.0, 1.0, 1.0, 1.0);
	scale = ofVec2f(1.,1.0);
	
	image = NULL;
	offaxis = NULL;
}

void ofxPuffersphereObject::makeTransparent()
{
	tint = ofColor(0,0,0,0);
}

bool ofxPuffersphereObject::isTransparent(){
	return tint.a < FLT_EPSILON;
}

void ofxPuffersphereObject::setup(float dimenions){
	targetSize.x = dimenions*2;
	targetSize.y = dimenions;
	
	canvasCoords.clear();
	canvasCoords.push_back(ofVec2f(0,0));
	canvasCoords.push_back(ofVec2f(targetSize.x,0));
	canvasCoords.push_back(ofVec2f(targetSize.x,targetSize.y));
	canvasCoords.push_back(ofVec2f(0,targetSize.y));
}
	
void ofxPuffersphereObject::draw(){
	
	if (isTransparent()) {
		return;
	}
	
	if(offaxis == NULL){
		ofLog(OF_LOG_ERROR, "ofxPuffersphere --- the shader is null!");
		return;
	}
	if(image == NULL){
		ofLog(OF_LOG_ERROR, "ofxPuffersphere --- the drawable is null!");
	}
	
	offaxis->begin();
	
	//should be 2:1, like 2100x1050 since the puffersphere we are working with draws to 1050x1050
	offaxis->setUniform2f("canvasDimensions", targetSize.x, targetSize.y); 
	offaxis->setUniform2f("textureDimensions", image->getTextureReference().getWidth(), image->getTextureReference().getHeight());

	offaxis->setUniform2f("textureScale", scale.x, scale.y);
	
	ofVec4f quatVec = rotation.asVec4();
	offaxis->setUniform4f("quat", quatVec.x,quatVec.y,quatVec.z,quatVec.w);
	
	ofColor colors[4] = { tint,tint,tint,tint }; //color arrays
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(ofVec2f), &(canvasCoords[0].x));
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, sizeof(ofColor), &(colors[0]).r);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), &(canvasCoords[0]).x);

	image->getTextureReference().bind();
	glDrawArrays(GL_QUADS, 0, 4);	
	image->getTextureReference().unbind();
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	offaxis->end();
}

void ofxPuffersphereObject::setTexture(ofBaseHasTexture* newTexture)
{
	if(image != NULL && textureIsOurs){
		delete image;
	}
	image = newTexture;
	textureIsOurs = false;
}

void ofxPuffersphereObject::loadImage(string filename)
{
	if(textureIsOurs && image != NULL){
		delete image;
		image = NULL;
	}
	
	ofImage* newimage = new ofImage();
	if(!newimage->loadImage(filename)){
		ofLog(OF_LOG_ERROR, "ofxPuffersphere --- image couldn't load!");
		delete newimage;
	}
	else{
		textureIsOurs = true;
		image = newimage;
	}
}

void ofxPuffersphereObject::loadVideo(string filename){
	if(textureIsOurs && image != NULL){
		delete image;
	}
	
	ofVideoPlayer* newvideo = new ofVideoPlayer();
	if(!newvideo->loadMovie(filename)){
		ofLog(OF_LOG_ERROR, "ofxPuffersphere --- video couldn't load!");
		delete newvideo;
	}
	else{
		textureIsOurs = true;
		ofLog(OF_LOG_VERBOSE, "ofxPuffersphere --- video loaded successfully.");
		image = newvideo;
	}	
}
	


