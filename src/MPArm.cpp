//
//  MPArm.cpp
//  SimpleExample
//
//  Created by TATSUHIKO SUZUKI M++ on 2016/12/27.
//
//

#include "MPArm.h"


void MPArm::setup(ofxBulletWorldRigid &world, float _base_x, float _base_y) {
    
    base = new ofxBulletBox();
    base->create(world.world, ofVec3f(0, 160/2, 0), 0, 500, 160, 500);
    base->setProperties(.25, .95);
    base->add();
    
    pan_a = new ofxBulletBox();
    pan_a->create(world.world, ofVec3f(0,160+190/2, 0), 0., 193.,190.,193.);
    pan_a->setProperties(.25,.95);
    pan_a->add();
    pan_a->enableKinematic();
    //pan_a->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_a = new ofxBulletBox();
    tilt_a->create(world.world, ofVec3f(0.,0., 0.), 1., 1000., 193., 193.);
    tilt_a->setProperties(.25,.95);
    tilt_a->add();
    tilt_a->enableKinematic();
    tilt_a->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_b = new ofxBulletBox();
    tilt_b->create(world.world, ofVec3f(0.,0., 0.), 1., 1000.f, 169.f, 169.f );
    tilt_b->setProperties(.25,.95);
    tilt_b->add();
    tilt_b->enableKinematic();
    tilt_b->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_c = new ofxBulletBox();
    tilt_c->create(world.world, ofVec3f(0.,0.,0.),1.,150.f,197.f,197.f);
    tilt_c->setProperties(.25,.95);
    tilt_c->add();
    tilt_c->enableKinematic();
    tilt_c->setActivationState(DISABLE_DEACTIVATION);
    
    pan_b = new ofxBulletBox();
    pan_b->create(world.world, ofVec3f(0.,0.,0.),1.,76.f,197.f,197.f);
    pan_b->setProperties(.25,.95);
    pan_b->add();
    pan_b->enableKinematic();
    pan_b->setActivationState(DISABLE_DEACTIVATION);
    
    base_x = _base_x;
    base_y = _base_y;
    
}

void MPArm::setOsc(float _pan_a, float _tilt_a, float _tilt_b) {
    
    receive_pan_a_oscData = _pan_a;
    receive_tilt_a_oscData = _tilt_a;
    receive_tilt_b_oscData = _tilt_b;
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
    //float newRot_pan_a = slider_pan_a*0.1;
    float newRot_pan_a = receive_pan_a_oscData;
    //float newRot_tilt_a = slider_tilt_a*0.1;
    float newRot_tilt_a = receive_tilt_a_oscData;
    //float newRot_tilt_b = slider_tilt_b*0.1;
    float newRot_tilt_b = receive_tilt_b_oscData;
    //float newRot_tilt_c = slider_tilt_c*0.1;
    //float newRot_pan_b = slider_pan_b*0.1;
    
    float newRot_tilt_c = 0;
    float newRot_pan_b = 0;
    
    // set the rotation of the bullet transform to that of the axis of the stored quaternion
    // and apply the new rotation
    
    btQuaternion pan_a_quat =  btQuaternion(btVector3(0.,1.0,0.),newRot_pan_a);
    btQuaternion tilt_a_quat = btQuaternion(btVector3(0.,0.,1.0),newRot_tilt_a);
    btQuaternion tilt_b_quat = btQuaternion(btVector3(0.,0.,1.0),newRot_tilt_b);
    btQuaternion tilt_c_quat = btQuaternion(btVector3(0.,0.,1.0),newRot_tilt_c);
    btQuaternion pan_b_quat =  btQuaternion(btVector3(1.0,0.,0),newRot_pan_b);
    
    
    trans_pan_a.setRotation(pan_a_quat);
    trans_tilt_a.setRotation(pan_a_quat * tilt_a_quat );
    trans_tilt_b.setRotation(pan_a_quat * tilt_a_quat * tilt_b_quat);
    trans_tilt_c.setRotation(pan_a_quat * tilt_a_quat * tilt_b_quat * tilt_c_quat);
    trans_pan_b.setRotation(pan_a_quat * tilt_a_quat * tilt_b_quat * tilt_c_quat * pan_b_quat);
    
    
    //tilt_a 座標計算
    
    float tilt_a_x = cos(newRot_tilt_a)*500.0f;
    float tilt_a_y = sin(newRot_tilt_a)*500.0f;
    
    float tilt_a_x_pan = cos(newRot_pan_a)*tilt_a_x;
    float tilt_a_z_pan = -sin(newRot_pan_a)*tilt_a_x;
    
    btVector3 vector_tilt_a = btVector3(tilt_a_x_pan+base_x, tilt_a_y+160+190+base_y, tilt_a_z_pan);
    
    float tilt_origin_x = tilt_a_x+cos(ofDegToRad(newRot_tilt_a))*500.0f;
    float tilt_origin_y = tilt_a_y+sin(ofDegToRad(newRot_tilt_a))*500.0f;
    
    //tilt_b　座標計算
    
    float tilt_b_local_x = cos(newRot_tilt_b)*500.0f+1000.0f;
    float tilt_b_local_y = sin(newRot_tilt_b)*500.0f;
    
    float tilt_b_global_x = tilt_b_local_x * cos(newRot_tilt_a) - tilt_b_local_y * sin(newRot_tilt_a);
    float tilt_b_global_y = tilt_b_local_x * sin(newRot_tilt_a) + tilt_b_local_y * cos(newRot_tilt_a);
    
    float tilt_b_global_x_pan = cos(newRot_pan_a)*tilt_b_global_x;
    float tilt_b_global_z_pan = -sin(newRot_pan_a)*tilt_b_global_x;
    
    float tilt_b_x = tilt_b_global_x_pan+base_x;
    float tilt_b_y = tilt_b_global_y+160+190+base_y;
    float tilt_b_z = tilt_b_global_z_pan;
    
    //tilt_c 座標計算
    float tilt_c_local_x = cos(newRot_tilt_c)*75.0f+1000.0f;
    float tilt_c_local_y = sin(newRot_tilt_c)*75.0f;
    
    float tilt_c_tilt_b_x = tilt_c_local_x * cos(newRot_tilt_b) - tilt_c_local_y * sin(newRot_tilt_b)+1000.0f;
    float tilt_c_tilt_b_y = tilt_c_local_x * sin(newRot_tilt_b) + tilt_c_local_y * cos(newRot_tilt_b);
    
    float tilt_c_global_x = tilt_c_tilt_b_x * cos(newRot_tilt_a) - tilt_c_tilt_b_y * sin(newRot_tilt_a);
    float tilt_c_global_y = tilt_c_tilt_b_x * sin(newRot_tilt_a) + tilt_c_tilt_b_y * cos(newRot_tilt_a);
    
    float tilt_c_global_x_pan = cos(newRot_pan_a)*tilt_c_global_x;
    float tilt_c_global_z_pan = -sin(newRot_pan_a)*tilt_c_global_x;
    
    float tilt_c_x = tilt_c_global_x_pan+base_x;
    float tilt_c_y = tilt_c_global_y+160+190+base_y;
    float tilt_c_z = tilt_c_global_z_pan;
    
    //pan_b 座標計算
    
    float pan_b_local_x = cos(newRot_tilt_c)*(75.0f+113.0f)+1000.0f;
    float pan_b_local_y = sin(newRot_tilt_c)*(75.0f+113.0f);
    
    float pan_b_tilt_b_x = pan_b_local_x * cos(newRot_tilt_b) - pan_b_local_y * sin(newRot_tilt_b)+1000.0f;
    float pan_b_tilt_b_y = pan_b_local_x * sin(newRot_tilt_b) + pan_b_local_y * cos(newRot_tilt_b);
    
    float pan_b_global_x = pan_b_tilt_b_x * cos(newRot_tilt_a) - pan_b_tilt_b_y * sin(newRot_tilt_a);
    float pan_b_global_y = pan_b_tilt_b_x * sin(newRot_tilt_a) + pan_b_tilt_b_y * cos(newRot_tilt_a);
    
    float pan_b_global_x_pan = cos(newRot_pan_a)*pan_b_global_x;
    float pan_b_global_z_pan = -sin(newRot_pan_a)*pan_b_global_x;
    
    float pan_b_x = pan_b_global_x_pan+base_x;
    float pan_b_y = pan_b_global_y+160+190+base_y;
    float pan_b_z = pan_b_global_z_pan;
    
    
    //setOrigin
    trans_pan_a.setOrigin(btVector3(0+base_x, 160+190/2+base_y, 0));
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
