//
//  MPArm.hpp
//  SimpleExample
//
//  Created by TATSUHIKO SUZUKI M++ on 2016/12/27.
//
//

#ifndef MPArm_hpp
#define MPArm_hpp

#include <stdio.h>
#include "ofxBullet.h"

class MPArm{
    public:
        void setup(ofxBulletWorldRigid world, float ground_x, float ground_y);
        void setOsc(float _pan_a, float _tilt_a, float _tilt_b);
        void update();
        void draw();
    
        ofxBulletBox				ground;
        ofxBulletBox*				base;
    
        ofxBulletBox*               pan_a;
        ofxBulletBox*               tilt_a;
        ofxBulletBox*               tilt_b;
        ofxBulletBox*               tilt_c;
        ofxBulletBox*               pan_b;
    
        float receive_pan_a_oscData;
        float receive_tilt_a_oscData;
        float receive_tilt_b_oscData;
    
        float base_x;
        float base_y;

};

#endif /* MPArm_hpp */
