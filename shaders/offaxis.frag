//uniform sampler2DRect bg;  //unused really. just used to control sampler size
uniform sampler2DRect tex; //our image we are drawing

uniform vec2 textureDimensions;
uniform vec2 canvasDimensions;      // width and height of the texture
uniform vec4 quat;
uniform vec2 textureScale;		

// ---------- CONSTANTS

float PI = 3.141592653589793;
float TWOPI = 6.283185307179586;

vec3 pointInSpace(vec2 p)  //u and v vary zero to one
{    
    float x = sin(p.y*PI) * cos(p.x*PI);
    float y = -sin(p.y*PI) * sin(p.x*PI);			// tig: changed to -sin
    float z = cos(p.y*PI);
    
    return vec3(x, y, z);
}

vec2 uvCoord(vec3 p)  //u and v vary zero to one
{
    float u = -0.25 + atan(p.x,p.y) / TWOPI;     // atan with 2 parameters; range: -PI to +PI
												 // tig: changed from 0.5 + atan...
    float v = acos(p.z) / PI;
    
    return vec2(u, v);
}

vec3 qtransform( vec4 q, vec3 v ){
    return v + 2.0*cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}

// ---------- SHADER LOGIC
void main()
{
    vec2 samplerPosSphereSpace = gl_TexCoord[0].xy;
    vec2 samplerPosNormalised = samplerPosSphereSpace / vec2(canvasDimensions.y, canvasDimensions.y) ;
	
    vec3 pointOnSphere = pointInSpace(samplerPosNormalised);
    pointOnSphere = qtransform(quat, pointOnSphere);
    vec2 newuv = uvCoord(pointOnSphere);
	
    vec2 offset = (textureDimensions * textureScale - canvasDimensions) / 2.0;

    vec2 samplerPos = mod(newuv * canvasDimensions/textureScale, canvasDimensions/textureScale) + offset;

    if(samplerPos.x < 0. || samplerPos.y < 0. || samplerPos.x > textureDimensions.x || samplerPos.y > textureDimensions.y){
		gl_FragColor = vec4(0.0);
	}
	else{
		gl_FragColor = gl_Color * texture2DRect(tex, samplerPos);
	}
}
