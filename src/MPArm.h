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

#define BASE_HEIGHT 160.f
#define PAN_A_HEIGHT 190.f
#define TILT_A_HEIGHT 1000.f
#define TILT_B_HEIGHT 1000.f
#define TILT_C_HEIGHT 150.f
#define PAN_B_HEIGHT 76.f

#define BASE_WIDTH 500.f
#define PAN_A_WIDTH 193.f
#define TILT_A_WIDTH 193.f
#define TILT_B_WIDTH 169.f
#define TILT_C_WIDTH 197.f
#define PAN_B_WIDTH 197.f

#define SCALE 0.5

class MPArm{
    public:
        void setup(ofxBulletWorldRigid &world, float ground_x, float ground_y);
        void setOsc(float _pan_a, float _tilt_a, float _tilt_b, float _tilt_c, float pan_b);
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
        float receive_tilt_c_oscData;
        float receive_pan_b_oscData;
    
        float base_x;
        float base_z;
    
        float base_height;
        float pan_a_height;
        float tilt_a_height;
        float tilt_b_height;
        float tilt_c_height;
        float pan_b_height;
    
        float base_width;
        float pan_a_width;
        float tilt_a_width;
        float tilt_b_width;
        float tilt_c_width;
        float pan_b_width;

};

#endif /* MPArm_hpp */
