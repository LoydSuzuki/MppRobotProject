#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
	
	camera.setPosition(ofVec3f(0, -20.f, -20.f));
	camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

	world.setup();
    world.setGravity(ofVec3f(0,0,0));
	world.enableGrabbing();
	//world.enableDebugDraw();
	world.setCamera(&camera);
    
    world.enableCollisionEvents();
    ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
	
	sphere = new ofxBulletSphere();
	sphere->create(world.world, ofVec3f(0, 0, 0), 0.01, .25);
    
	//sphere->add();
    
    TestData boxdata;
	
	box = new ofxBulletBox();
    box->create(world.world, ofVec3f(10, 0, 0), 0, 10, .5, 10);
    box->setProperties(.25, .95);
    box->getRigidBody()->setUserPointer(&boxdata);
    box->add();
    box->activate();
    box->setActivationState(DISABLE_DEACTIVATION);
	
	cone = new ofxBulletCone();
	cone->create(world.world, ofVec3f(-1, -1, .2), .2, .4, 1.);
	//cone->add();
	
	capsule = new ofxBulletCapsule();
	capsule->create(world.world, ofVec3f(1, -2, -.2), .4, .8, 1.2);
	//capsule->add();
	
	cylinder = new ofxBulletCylinder();
	cylinder->create(world.world, ofVec3f(0, -2.4, 0), .8, .9, 1.8);
	//cylinder->add();
    
    // set quaternion, so we know which way is up //
    ofQuaternion groundQuat;
    groundQuat.set(0, 1, 0, 1.);
    ground.create( world.world, ofVec3f(0., 5.5, 0.), groundQuat, 0., 10.f, 1.f, 10.f );
    ground.setProperties(.25, .95);
    ground.add();
    // allows manipulation of object. The object will not react to other objects, but will apply forces to them //
    // just like a static object, except you can move it around //
    ground.enableKinematic();
    ground.setActivationState(DISABLE_DEACTIVATION);
    
    
    
    ofQuaternion tilt01Quat;
    tilt01Quat.set(0,1,0,1);
    tilt_a = new ofxBulletBox();
    tilt_a->create( world.world, ofVec3f(0.,0., 0.), tilt01Quat, 1., 10.f, 0.2f, 0.2f );
    tilt_a->setProperties(.25,.95);
    tilt_a->add();
    tilt_a->enableKinematic();
    tilt_a->setActivationState(DISABLE_DEACTIVATION);
    
    tilt_b = new ofxBulletBox();
    tilt_b->create( world.world, ofVec3f(0.,0., 0.), tilt01Quat, 1., 10.f, 0.2f, 0.2f );
    tilt_b->setProperties(.25,.95);
    tilt_b->add();
    tilt_b->enableKinematic();
    tilt_b->setActivationState(DISABLE_DEACTIVATION);
    
    /*
    tilt_as = new ofxBulletCustomShape();
    tilt_as->create(world.world, ofVec3f(0.,0.,0.),0.);
    tilt_as->addShape(tilt_a->getRigidBody()->getCollisionShape(), ofVec3f(0.,5.,0.));
    tilt_as->addShape(tilt_b->getRigidBody()->getCollisionShape(), ofVec3f(0.,-5.,0.));
    tilt_as->add();
    tilt_as->enableKinematic();
    tilt_as->setActivationState(DISABLE_DEACTIVATION);
    */
    
    //gui
    gui.setup("panel");
    gui.add(angle_a.setup("angle_a",0.,-50.,50.));
    gui.add(angle_b.setup("angle_b",0.,-50.,50.));
    
    //extern ContactProcessedCallback	gContactProcessedCallback;
    extern ContactAddedCallback	gContactAddedCallback;
    
    // Èñ¢Êï∞ÂÆ£Ë®Ä
    bool ContactCallback( btManifoldPoint& manifold, void* object0, void* object1 );
    
    // Bullet„ÅÆÈñ¢Êï∞„Éù„Ç§„É≥„Çø„Å´Ë®≠ÂÆö„Åô„Çã
    gContactProcessedCallback = ContactCallback;

}

bool ContactCallback( btManifoldPoint& manifold, void* object0, void* object1 )
{
    
    /*
    //„Åì„ÅÆ‰∏≠„Å´ÂÄãÂà•„ÅÆÂá¶ÁêÜ„ÇíÊõ∏„Åè
    btRigidBody* pBody0 = (btRigidBody*)object0;
    btRigidBody* pBody1 = (btRigidBody*)object1;
    
    TestData* pUserData0 = (TestData*)pBody0->getUserPointer();
    TestData* pUserData1 = (TestData*)pBody1->getUserPointer();
    
    // „Ç´„Ç¶„É≥„Éà
    if(pUserData0) pUserData0->count++;
    if(pUserData1) pUserData1->count++;
    
    
    return true;
    */
    
    cout << "ContactCallBack " <<endl;
}

//--------------------------------------------------------------
void ofApp::update() {
	
    world.update();
    //ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    // store the position of the ground //
    ofVec3f pos = ground.getPosition();
    
    // create a bullet transform object and set the position to that of the object //
    btTransform trans_a;
    btTransform trans_b;
    //trans.setOrigin( btVector3( btScalar(pos.x), angle*0.5, btScalar(pos.z)) );
    trans_a.setIdentity();
    trans_b.setIdentity();
    //trans.setOrigin( btVector3( 3.0f, 3.0f, 3.0f) );
    
    // get the rotation quaternion from the ground //
    ofQuaternion rotQuat = tilt_a->getRotationQuat();
    // print out the angle
    cout << "rotation " << angle_a << endl;
    float newRot_a = rotQuat.w();
    float newRot_b = rotQuat.w();
    // rotate it a bit //
    newRot_a = angle_a*0.1;
    newRot_b = angle_b*0.1;
    // clamp values between PI and -PI so there is no jumping //
    //if(newRot >= PI) newRot -= PI;
    //if(newRot <= 0) newRot += PI;
    
    // set the rotation of the bullet transform to that of the axis of the stored quaternion
    // and apply the new rotation
    trans_a.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), 1.0f), newRot_a) );
    trans_b.setRotation( btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), 1.0f), newRot_a) * btQuaternion(btVector3(rotQuat.x(), rotQuat.y(), 1.0f), newRot_b));
    float tilt_a_x = cos(newRot_a)*5.0f-5.0;
    float tilt_a_y = sin(newRot_a)*5.0f;
    
    float tilt_origin_x = tilt_a_x+cos(newRot_a)*5.0f;
    float tilt_origin_y = tilt_a_y+sin(newRot_a)*5.0f;
    
    //先端座標の計算
    
    float tilt_b_local_x = cos(newRot_b)*5.0f+10.0f;
    float tilt_b_local_y = sin(newRot_b)*5.0f;
    
    float tilt_b_global_x = tilt_b_local_x * cos(newRot_a) - tilt_b_local_y * sin(newRot_a);
    float tilt_b_global_y = tilt_b_local_x * sin(newRot_a) + tilt_b_local_y * cos(newRot_a);
    
    float tilt_b_x = tilt_b_global_x-5.0f;
    float tilt_b_y = tilt_b_global_y;
    
    
    //float tilt_b_x = tilt_origin_x;
    //float tilt_b_y = tilt_origin_y;

    trans_a.setOrigin( btVector3( tilt_a_x, tilt_a_y, 0.0f));
    trans_b.setOrigin(btVector3( tilt_b_x, tilt_b_y, 0.0f));
    // apply the transform to the rigid body //
    //tilt_a->getRigidBody()->setCenterOfMassTransform(trans);
    tilt_a->getRigidBody()->setCenterOfMassTransform(trans_a);
    tilt_a->getRigidBody()->getMotionState()->setWorldTransform(trans_a);
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    tilt_a->activate();
    
    tilt_b->getRigidBody()->setCenterOfMassTransform(trans_b);
    tilt_b->getRigidBody()->getMotionState()->setWorldTransform(trans_b);
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    tilt_b->activate();
    
    //tilt_as->getRigidBody()->setCenterOfMassTransform(trans);
    //tilt_as->getRigidBody()->getMotionState()->setWorldTransform( trans );
    // tell the ofxBulletWorldRigid that we have moved rigid body and it should update the collision object //
    //tilt_as->activate();

    
   }

//--------------------------------------------------------------
void ofApp::draw() {
	glEnable( GL_DEPTH_TEST );
	camera.begin();
	
	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	world.drawDebug();
	
    if(ground_colliding == TRUE){
        ofSetColor(255,0,0);
        ground.draw();
    }
    else{
        ofSetColor(100,100,100);
        ground.draw();
    }

	
	ofSetColor(225, 225, 225);
	//sphere->draw();
	
	ofSetColor(225, 225, 225);
	box->draw();
	
	ofSetColor(225, 225, 225);
	//cylinder->draw();
	
	ofSetColor(225, 225, 225);
	//capsule->draw();
	
	ofSetColor(225, 225, 225);
	//cone->draw();
    
    ofSetColor(225, 225, 225);
    tilt_a->draw();
    tilt_b->draw();
	
	camera.end();
    
    glDisable( GL_DEPTH_TEST );
    
    gui.draw();
    
    ground_colliding = FALSE;
}

//--------------------------------------------------------------


void ofApp::onCollision(ofxBulletCollisionData& cdata) {
    
    cout << "ContactCallBack " <<endl;
    
    /*
     for(int j = 0; j < bounds.size(); j++) {
     if(*bounds[j] == cdata) {
     return;
     }
     }
     
     for (int i = 0; i < shapes.size(); i++) {
     if(*shapes[i] == cdata) {
     bColliding[i] = true;
     }
     }*/
    
    if(*box == cdata){
        ground_colliding = TRUE;
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 
	
}
