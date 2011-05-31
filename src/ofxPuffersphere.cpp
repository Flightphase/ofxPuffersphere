/*
 *  ofxPuffersphere.cpp
 *  NMMPuffersphereDesignPrototype
 *
 *  Created by Jim on 5/11/11.
 *  Copyright 2011 FlightPhase. All rights reserved.
 *
 */

#include "ofxPuffersphere.h"

ofxPuffersphere::ofxPuffersphere()
{
	offaxisLocation = "offaxis";
	spherizeLocation = "spherize";
	hasBegun = false;
	shaderLoaded = false;
	renderForPuffersphere = false;
}

ofxPuffersphere::~ofxPuffersphere()
{
	for(int i = 0; i < objects.size(); i++){
		delete objects[i];
	}
	objects.clear();
}

void ofxPuffersphere::setup(float s)
{
	size = s;
	canvas.setup(size*2, size, GL_RGBA, 4);

	//TODO: collapse shaders into one
	offaxis.setup(offaxisLocation);	  //NMM_SHARED_RESOURCE_PREFIX+"shaders/puffersphere/offaxis"
 	spherize.setup(spherizeLocation); //NMM_SHARED_RESOURCE_PREFIX+"shaders/puffersphere/spherize"

	sphereShaderSettings.projection_method = 3;
}


#ifdef USE_SIMPLE_GUI
void ofxPuffersphere::addGuiParams()
{
	gui.addPage("Custom Pufferspher Settings");
	gui.addTitle("Render For Puffersphere", renderForPuffersphere);
	gui.addSlider("fov", sphereShaderSettings.fov, 1., 359.);
	gui.addSlider("Z", sphereShaderSettings.Z, 0., 6000);
}
#endif

ofxPuffersphereObject* ofxPuffersphere::createObject()
{
	ofxPuffersphereObject* object = new ofxPuffersphereObject();
	object->offaxis = getShaderReference();
	object->setup(size);
	objects.push_back( object );
	return object;
}

ofxPuffersphereObject* ofxPuffersphere::createObject(string imagePath)
{
	ofxPuffersphereObject* object = createObject();
	object->loadImage(imagePath);	
	return object;
}

ofxPuffersphereObject* ofxPuffersphere::createObjectFromVideoPath(string videoPath)
{
	ofxPuffersphereObject* object = createObject();
	object->loadVideo(videoPath);
	return object;
}


ofShader* ofxPuffersphere::getShaderReference() //return ref. to the offaxis location
{
	return &offaxis;
}

void ofxPuffersphere::render()
{
	begin();
	renderAllObjects();	
	end();
}

void ofxPuffersphere::begin()
{
	if(!hasBegun){
		hasBegun = true;
		canvas.begin();
		ofClear(255, 255, 255, 255);
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        ofSetColor(255,255,255);
	}
}

void ofxPuffersphere::end()
{
	if(hasBegun){
		glPopAttrib();
		canvas.end();
		hasBegun = false;
	}
}

void ofxPuffersphere::renderAllObjects()
{

	for(int i = 0; i < objects.size(); i++){
		objects[i]->draw();
	}

}

void ofxPuffersphere::draw() 
{
	//draws for puffersphere as target
	if(renderForPuffersphere){
		spherize.begin();
		spherize.setUniform2f("imageSize", float(canvas.getWidth()), float(canvas.getHeight()));
		spherize.setUniform1i("projection_method", sphereShaderSettings.projection_method);
		spherize.setUniform1f("fov", sphereShaderSettings.fov);
		spherize.setUniform1f("Z", sphereShaderSettings.Z);
		//centered
		canvas.draw(ofGetWidth()/2.-canvas.getWidth()/2., ofGetHeight()/2.-canvas.getHeight()/2.);
		spherize.end();
	}
	//draw flat for preview on screen
	else {
		ofPushMatrix();
		//TODO: scale viewport
		glScalef(.5, .5, .5);
		canvas.draw(0, 0);
		ofPopMatrix();		
	}
}

float ofxPuffersphere::getSize()
{
	return size;
}

void ofxPuffersphere::drawSphere(ofVec3f position)
{
    
    float radius = 8;
	int segments = 100;
    
	// tig: this was lifted from Cinder.
	// https://github.com/cinder/Cinder/blob/master/src/cinder/gl/gl.cpp
	
	// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/spheremap/  Paul Bourke's sphere code
	// We should weigh an alternative that reduces the batch count by using GL_TRIANGLES instead
	if( segments < 0 )
		return;
	
    canvas.getTexture(0).bind();
    
	float verts[(segments+1)*2*3];
	float normals[(segments+1)*2*3];
	float texCoords[(segments+1)*2*2];
	
	// TG. not pushing/popping this bit leads to weird things in the fbo drawn after
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    ofDisableAlphaBlending();
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, verts );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
	glEnableClientState( GL_NORMAL_ARRAY );
	glNormalPointer( GL_FLOAT, 0, normals );
	
	for( int j = 0; j < segments / 2; j++ ) {
		float theta1 = j * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		float theta2 = (j + 1) * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		
		for( int i = 0; i <= segments; i++ ) {
			ofVec3f e, p;
			float theta3 = i * 2 * 3.14159f / segments;
			
			e.x = cos( theta1 ) * cos( theta3 );
			e.y = sin( theta1 );
			e.z = cos( theta1 ) * sin( theta3 );
			p = e * radius + position;
			normals[i*3*2+0] = e.x; normals[i*3*2+1] = e.y; normals[i*3*2+2] = e.z;
			texCoords[i*2*2+0] = (1.0 - (i / (float)segments))*canvas.getWidth();
            texCoords[i*2*2+1] = (0.999f - 2 * j / (float)segments)*canvas.getHeight();
			verts[i*3*2+0] = p.x; verts[i*3*2+1] = p.y; verts[i*3*2+2] = p.z;
			
			e.x = cos( theta2 ) * cos( theta3 );
			e.y = sin( theta2 );
			e.z = cos( theta2 ) * sin( theta3 );
			p = e * radius + position;
			normals[i*3*2+3] = e.x; normals[i*3*2+4] = e.y; normals[i*3*2+5] = e.z;
			texCoords[i*2*2+2] = (1.0 - (i / (float)segments)) * canvas.getWidth(); 
            texCoords[i*2*2+3] = (0.999f - 2 * ( j + 1 ) / (float)segments) * canvas.getHeight();
			verts[i*3*2+3] = p.x; verts[i*3*2+4] = p.y; verts[i*3*2+5] = p.z;
		}
		glDrawArrays( GL_TRIANGLE_STRIP, 0, (segments + 1)*2 );
	}
	
    canvas.getTexture(0).unbind();
    
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	ofEnableAlphaBlending();	

	glPopAttrib();


}
