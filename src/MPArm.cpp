//
//  MPArm.cpp
//  SimpleExample
//
//  Created by TATSUHIKO SUZUKI M++ on 2016/12/27.
//
//

#include "MPArm.h"


void MPArm::setup(ofxBulletWorldRigid &world, float _base_x, float _base_z, int _base_rotation) {
    
    base_x = _base_x;
    base_z = _base_z;
    base_rotation = _base_rotation;
    
    base_height = BASE_HEIGHT*SCALE;
    pan_a_height = PAN_A_HEIGHT*SCALE;
    tilt_a_height = TILT_A_HEIGHT*SCALE;
    tilt_b_height = TILT_B_HEIGHT*SCALE;
    tilt_c_height = TILT_C_HEIGHT*SCALE;
    pan_b_height = PAN_B_HEIGHT*SCALE;
    
    base_width = BASE_WIDTH*SCALE;
    pan_a_width = PAN_A_WIDTH*SCALE;
    tilt_a_width = TILT_A_WIDTH*SCALE;
    tilt_b_width = TILT_B_WIDTH*SCALE;
    tilt_c_width = TILT_C_WIDTH*SCALE;
    pan_b_width = PAN_B_WIDTH*SCALE;
    
    base = new ofxBulletBox();
    base->create(world.world, ofVec3f(0+base_x, base_height/2, 0+base_z), 0, base_width, base_height, base_width);
    //base->setProperties(.25, .95);
    base->add();
    
    pan_a = new ofxBulletBox();
    pan_a->create(world.world, ofVec3f(0,base_height+pan_a_height/2, 0), 0., pan_a_width,pan_a_height,pan_a_width);
    pan_a->setProperties(.25,.95);
    pan_a->add();
    pan_a->enableKinematic();
    //pan_a->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_a = new ofxBulletBox();
    tilt_a->create(world.world, ofVec3f(0.,0.,0.), 1., tilt_a_height, tilt_a_width, tilt_a_width);
    //tilt_a->setProperties(.25,.95);
    tilt_a->add();
    tilt_a->enableKinematic();
    tilt_a->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_b = new ofxBulletBox();
    tilt_b->create(world.world, ofVec3f(0.,0., 0.), 1., tilt_a_height, tilt_b_width, tilt_b_width );
    //tilt_b->setProperties(.25,.95);
    tilt_b->add();
    tilt_b->enableKinematic();
    tilt_b->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_c = new ofxBulletBox();
    tilt_c->create(world.world, ofVec3f(0.,0.,0.),1.,tilt_c_height,tilt_c_width,tilt_c_width);
    //tilt_c->setProperties(.25,.95);
    tilt_c->add();
    tilt_c->enableKinematic();
    tilt_c->setActivationState(DISABLE_DEACTIVATION);
    
    pan_b = new ofxBulletBox();
    pan_b->create(world.world, ofVec3f(0.,0.,0.),1.,pan_b_height,pan_b_width,pan_b_width);
    //pan_b->setProperties(.25,.95);
    pan_b->add();
    pan_b->enableKinematic();
    pan_b->setActivationState(DISABLE_DEACTIVATION);
    
}

//oscデータをMPArmに読み込むための関数
void MPArm::setOsc(float _pan_a, float _tilt_a, float _tilt_b, float _tilt_c, float _pan_b) {
    
    receive_pan_a_oscData = _pan_a;
    receive_tilt_a_oscData = _tilt_a;
    receive_tilt_b_oscData = _tilt_b;
    receive_tilt_c_oscData = _tilt_c;
    receive_pan_b_oscData = _pan_b;
}

void MPArm::update(){
    
    // create a bullet transform object and set the position to that of the object //
    btTransform trans_pan_a;
    btTransform trans_tilt_a;
    btTransform trans_tilt_b;
    btTransform trans_tilt_c;
    btTransform trans_pan_b;
    
    //setIdentity(初期化)
    trans_pan_a.setIdentity();
    trans_tilt_a.setIdentity();
    trans_tilt_b.setIdentity();
    trans_tilt_c.setIdentity();
    trans_pan_b.setIdentity();
    
    // get the rotation quaternion from the ground //
    ofQuaternion rotQuat = tilt_a->getRotationQuat();
    
    // rotate it a bit //
    float rotation_base = ofDegToRad(base_rotation);
    //float newRot_pan_a = slider_pan_a;
    float rotation_pan_a = ofDegToRad(receive_pan_a_oscData*360);
    //float newRot_tilt_a = slider_tilt_a;
    float rotation_tilt_a = ofDegToRad(receive_tilt_a_oscData*120+90-120/2);
    //float newRot_tilt_b = slider_tilt_b;
    float rotation_tilt_b = ofDegToRad(receive_tilt_b_oscData*270-270/2);
    //float newRot_tilt_c = slider_tilt_c;
    float rotation_tilt_c = ofDegToRad(receive_tilt_c_oscData*270-270/2);
    //float newRot_pan_b = slider_pan_b;
    float rotation_pan_b = ofDegToRad(receive_pan_b_oscData*360);

    
    // set the rotation of the bullet transform to that of the axis of the stored quaternion
    // and apply the new rotation
    
    btQuaternion base_quat =   btQuaternion(btVector3(0.,1.0,0.),rotation_base);
    btQuaternion pan_a_quat =  btQuaternion(btVector3(0.,1.0,0.),rotation_pan_a);
    btQuaternion tilt_a_quat = btQuaternion(btVector3(0.,0.,1.0),rotation_tilt_a);
    btQuaternion tilt_b_quat = btQuaternion(btVector3(0.,0.,1.0),rotation_tilt_b);
    btQuaternion tilt_c_quat = btQuaternion(btVector3(0.,0.,1.0),rotation_tilt_c);
    btQuaternion pan_b_quat =  btQuaternion(btVector3(1.0,0.,0),rotation_pan_b);
    
    
    trans_pan_a.setRotation(base_quat * pan_a_quat);
    trans_tilt_a.setRotation(base_quat * pan_a_quat * tilt_a_quat );
    trans_tilt_b.setRotation(base_quat * pan_a_quat * tilt_a_quat * tilt_b_quat);
    trans_tilt_c.setRotation(base_quat * pan_a_quat * tilt_a_quat * tilt_b_quat * tilt_c_quat);
    trans_pan_b.setRotation(base_quat * pan_a_quat * tilt_a_quat * tilt_b_quat * tilt_c_quat * pan_b_quat);
    
    
    //tilt_a 座標計算
    
    float tilt_a_x = cos(rotation_tilt_a)*tilt_a_height/2;
    float tilt_a_y = sin(rotation_tilt_a)*tilt_a_height/2;
    
    float tilt_a_x_pan = cos(rotation_base + rotation_pan_a)*tilt_a_x;
    float tilt_a_z_pan = -sin(rotation_base + rotation_pan_a)*tilt_a_x;
    
    btVector3 vector_tilt_a = btVector3(tilt_a_x_pan+base_x, tilt_a_y+base_height+pan_a_height, tilt_a_z_pan+base_z);
    
    //float tilt_origin_x = tilt_a_x+cos(newRot_tilt_a)*500.0f;
    //float tilt_origin_y = tilt_a_y+sin(newRot_tilt_a)*500.0f;
    
    //tilt_b　座標計算
    float tilt_b_local_x = cos(rotation_tilt_b)*tilt_b_height/2+tilt_a_height;
    float tilt_b_local_y = sin(rotation_tilt_b)*tilt_b_height/2;
    
    float tilt_b_global_x = tilt_b_local_x * cos(rotation_tilt_a) - tilt_b_local_y * sin(rotation_tilt_a);
    float tilt_b_global_y = tilt_b_local_x * sin(rotation_tilt_a) + tilt_b_local_y * cos(rotation_tilt_a);
    
    float tilt_b_global_x_pan = cos(rotation_base + rotation_pan_a)*tilt_b_global_x;
    float tilt_b_global_z_pan = -sin(rotation_base + rotation_pan_a)*tilt_b_global_x;
    
    float tilt_b_x = tilt_b_global_x_pan+base_x;
    float tilt_b_y = tilt_b_global_y+base_height+pan_a_height;
    float tilt_b_z = tilt_b_global_z_pan+base_z;
    
    //tilt_c 座標計算
    float tilt_c_local_x = cos(rotation_tilt_c)*tilt_c_height/2+tilt_b_height;
    float tilt_c_local_y = sin(rotation_tilt_c)*tilt_c_height/2;
    
    float tilt_c_tilt_b_x = tilt_c_local_x * cos(rotation_tilt_b) - tilt_c_local_y * sin(rotation_tilt_b)+tilt_a_height;
    float tilt_c_tilt_b_y = tilt_c_local_x * sin(rotation_tilt_b) + tilt_c_local_y * cos(rotation_tilt_b);
    
    float tilt_c_global_x = tilt_c_tilt_b_x * cos(rotation_tilt_a) - tilt_c_tilt_b_y * sin(rotation_tilt_a);
    float tilt_c_global_y = tilt_c_tilt_b_x * sin(rotation_tilt_a) + tilt_c_tilt_b_y * cos(rotation_tilt_a);
    
    float tilt_c_global_x_pan = cos(rotation_base + rotation_pan_a)*tilt_c_global_x;
    float tilt_c_global_z_pan = -sin(rotation_base + rotation_pan_a)*tilt_c_global_x;
    
    float tilt_c_x = tilt_c_global_x_pan+base_x;
    float tilt_c_y = tilt_c_global_y+base_height+pan_a_height;
    float tilt_c_z = tilt_c_global_z_pan+base_z;
    
    //pan_b 座標計算
    float pan_b_local_x = cos(rotation_tilt_c)*(tilt_c_height/2+(tilt_c_height+pan_b_height)/2)+tilt_b_height;
    float pan_b_local_y = sin(rotation_tilt_c)*(tilt_c_height/2+(tilt_c_height+pan_b_height)/2);
    
    float pan_b_tilt_b_x = pan_b_local_x * cos(rotation_tilt_b) - pan_b_local_y * sin(rotation_tilt_b)+tilt_a_height;
    float pan_b_tilt_b_y = pan_b_local_x * sin(rotation_tilt_b) + pan_b_local_y * cos(rotation_tilt_b);
    
    float pan_b_global_x = pan_b_tilt_b_x * cos(rotation_tilt_a) - pan_b_tilt_b_y * sin(rotation_tilt_a);
    float pan_b_global_y = pan_b_tilt_b_x * sin(rotation_tilt_a) + pan_b_tilt_b_y * cos(rotation_tilt_a);
    
    float pan_b_global_x_pan = cos(rotation_base + rotation_pan_a)*pan_b_global_x;
    float pan_b_global_z_pan = -sin(rotation_base + rotation_pan_a)*pan_b_global_x;
    
    float pan_b_x = pan_b_global_x_pan+base_x;
    float pan_b_y = pan_b_global_y+base_height+pan_a_height;
    float pan_b_z = pan_b_global_z_pan+base_z;
    
    
    //setOrigin
    trans_pan_a.setOrigin(btVector3(0+base_x, base_height+pan_a_height/2, base_z));
    trans_tilt_a.setOrigin(vector_tilt_a);
    trans_tilt_b.setOrigin(btVector3( tilt_b_x, tilt_b_y, tilt_b_z));
    trans_tilt_c.setOrigin(btVector3( tilt_c_x, tilt_c_y, tilt_c_z));
    trans_pan_b.setOrigin(btVector3( pan_b_x, pan_b_y, pan_b_z));
    
    pan_a->getRigidBody()->setCenterOfMassTransform(trans_pan_a);
    pan_a->getRigidBody()->getMotionState()->setWorldTransform(trans_pan_a);
    pan_a->activate();
    
    tilt_a->getRigidBody()->setCenterOfMassTransform(trans_tilt_a);
    tilt_a->getRigidBody()->getMotionState()->setWorldTransform(trans_tilt_a);
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    tilt_a->activate();
    
    tilt_b->getRigidBody()->setCenterOfMassTransform(trans_tilt_b);
    tilt_b->getRigidBody()->getMotionState()->setWorldTransform(trans_tilt_b);
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    tilt_b->activate();
    
    tilt_c->getRigidBody()->setCenterOfMassTransform(trans_tilt_c);
    tilt_c->getRigidBody()->getMotionState()->setWorldTransform(trans_tilt_c);
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    tilt_c->activate();
    
    pan_b->getRigidBody()->setCenterOfMassTransform(trans_pan_b);
    pan_b->getRigidBody()->getMotionState()->setWorldTransform(trans_pan_b);
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    pan_b->activate();

}

void MPArm::draw(){
    
    ofSetColor(225, 225, 225);
    base->draw();
    
    ofSetColor(225, 225, 225);
    pan_a->draw();
    tilt_a->draw();
    tilt_b->draw();
    tilt_c->draw();
    pan_b->draw();

}
