//
//  InteractiveCircle.h
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 21/02/2017.
//
//

#pragma once
#include "ofMain.h"
#include "ofxOssia.h"

class InteractiveCircle {
    
public :

    InteractiveCircle(){}
    InteractiveCircle(ossia::ParameterGroup _parent_node);
    
//    ~InteractiveCircle(){
//        _colorParams.clearChild();
//        _sizeParams.clearChild();
//        _circleParams.clearChild();
//    }
    
    InteractiveCircle& setup(ossia::ParameterGroup _parent_node);

    void update();
    void draw();
    
    inline ossia::ParameterGroup & getCircleParams() {
        return _circleParams;
    }
    
private:

    ossia::ParameterGroup _circleParams;

        ossia::ParameterGroup _sizeParams;
            ossia::Parameter<float> _radius;
            ossia::Parameter<ofVec2f> _position;

        ossia::ParameterGroup _colorParams;
            ossia::Parameter<ofColor> _color;
            ossia::Parameter<bool> _fill;

};
