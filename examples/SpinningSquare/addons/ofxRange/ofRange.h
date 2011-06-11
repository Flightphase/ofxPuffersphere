/*
 *  ofRange.h
 *  SFEditor
 *
 *  Created by Jim on 1/15/11.
 *  Copyright 2011 FlightPhase. All rights reserved.
 *
 */

//JG Added for slider ranges
//----------------------------------------------------------
// ofRange
//----------------------------------------------------------
#pragma once

class ofRange {
public:
	virtual ~ofRange(){}
	
	ofRange( float _min=0.0f, float _max=0.0f){
		min = _min;
		max = _max;
	}
	
	void set(float _min, float _max){
		min = _min;
		max = _max;
	}
	
	void setMin(float _min){
		min = MIN(_min, max);
	}
	
	void setMax(float _max){
		max = MAX(min, _max);
	}
	
    union  {
        struct {
            float min;
            float max;
        };
        float r[2];
    };	
};
