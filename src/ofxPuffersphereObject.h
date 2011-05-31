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

#pragma once
#include "ofMain.h"


class virtualImage : public ofBaseHasTexture {
  public:
	ofTexture* tex;
	
	ofTexture& getTextureReference(){
		return *tex;
	};
	
	void setUseTexture(bool useTex=true){
		
	};


};

class ofxPuffersphereObject
{
  public:
	ofVec2f targetSize;
	ofQuaternion rotation;
	ofBaseHasTexture* image; //can be an ofFbo or ofImage, or any other drawable
	ofShader* offaxis; //please share me between all other sphere objects
    
	ofColor tint; // can be used to fade object out
	ofVec2f scale;
	
	ofxPuffersphereObject(){
		rotation = ofQuaternion(0,0,0,1);
		tint = ofColor(1.0, 1.0, 1.0, 1.0);
		scale.x= scale.y = 1.;
		
		image = NULL;
		offaxis = NULL;
		isA = "undefined";
	}
	
	void makeTransparent()
	{

		tint = ofColor(0,0,0,0);
	}
	
	bool isTransparent(){
		return (tint.r==0 && tint.g==0 && tint.b==0 && tint.a==0);
	}
	
	void setup(float dimenions = 1050){
		targetSize.x = dimenions*2;
		targetSize.y = dimenions;
		
		canvasCoords.clear();
		canvasCoords.push_back(ofVec2f(0,0));
		canvasCoords.push_back(ofVec2f(targetSize.x,0));
		canvasCoords.push_back(ofVec2f(targetSize.x,targetSize.y));
		canvasCoords.push_back(ofVec2f(0,targetSize.y));
	}
		
	void draw(){
		
		if (!isTransparent()) {
		
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
			//just the size of the image we are gooing to draw
			if (isA=="video") {
				offaxis->setUniform2f("textureDimensions",  ((ofVideoPlayer*)image)->getWidth(), 
									  ((ofVideoPlayer*)image)->getHeight());
				
			} else {
				offaxis->setUniform2f("textureDimensions", image->getTextureReference().getWidth(), image->getTextureReference().getHeight());
			}

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


			if (isA =="video") {
				((ofVideoPlayer*)image)->getTextureReference().bind();
				glPushMatrix();
				glDrawArrays(GL_QUADS, 0, 4);
				glPopMatrix();
				((ofVideoPlayer*)image)->getTextureReference().unbind();
			} else {
				image->getTextureReference().bind();
				glDrawArrays(GL_QUADS, 0, 4);
				image->getTextureReference().unbind();
			}
			
			
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			
			offaxis->end();
		}

	}
	
	void setImage(ofImage* newImage)
	{
		image = (ofBaseHasTexture*)newImage;
		isA = "image";
	}

	void setTexture(ofBaseHasTexture* newTexture)
	{
		image = newTexture;
		isA = "texture";
	}

	void setTexture(ofTexture* newTexture)
	{
		// careful, if you don't call delete image before calling this function, you have a leak!

		if(image != NULL){
			delete image;
			image = NULL;
			isA = "undefined";
		}
		
		virtualImage * vI = new virtualImage();
		vI->tex = newTexture;
		
		image = (ofBaseHasTexture*)vI;
		isA = "texture";
	}
	
	void loadImage(string filename)
	{
		if(image != NULL){
			delete image;
			image = NULL;
			isA = "undefined";
		}
		
		ofImage* newimage = new ofImage();
		if(!newimage->loadImage(filename)){
			ofLog(OF_LOG_ERROR, "ofxPuffersphere --- image couldn't load!");
			delete newimage;
		}
		else{
			image = (ofBaseHasTexture*)newimage;
			isA = "image";
		}
	}
	
	void setVideo(ofVideoPlayer* newVideo){
		image = (ofBaseHasTexture*)newVideo;
		newVideo->setUseTexture(true);
	}
	
	void loadVideo(string filename){
		if(image != NULL){
			delete image;
			image = NULL;
			isA = "undefined";
		}
		
		ofVideoPlayer* newvideo = new ofVideoPlayer();
		if(!newvideo->loadMovie(filename)){
			ofLog(OF_LOG_ERROR, "ofxPuffersphere --- video couldn't load!");
			delete newvideo;
		}
		else{
			ofLog(OF_LOG_VERBOSE, "ofxPuffersphere --- video loaded successfully.");
			newvideo->setUseTexture(true);
			image = (ofBaseHasTexture*)newvideo;
			isA = "video";
		}
		
	}
	
  private:
	vector<ofVec2f> canvasCoords;
	string isA;	// remember the original type of the object.
};

