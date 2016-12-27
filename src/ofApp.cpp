#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
    
    //easyCam setup
    easyCam.setDistance(3000);
    
    //world setup
	world.setup(); //重力は0
    world.setGravity(ofVec3f(0,0,0));
	//world.enableGrabbing();
    world.enableCollisionEvents(); //衝突判定
    ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
    
    //地面(ground)setup
    ground.create( world.world, ofVec3f(0.,-0.5, 0.), 0., 5000.f, 1.0f, 5000.f );
    ground.setProperties(.25, .95);
    ground.add();
    // allows manipulation of object. The object will not react to other objects, but will apply forces to them //
    
    
    //MPArm
    //土台(base)setup
    
    arm.setup(world,0,0);
	
    receiver.setup(PORT);
    
    //gui
    gui.setup("panel");
    gui.add(slider_pan_a.setup("pan_a",0.,-50.,50.));
    gui.add(slider_tilt_a.setup("tilt_a",0.,-50.,50.));
    gui.add(slider_tilt_b.setup("tilt_b",0.,-50.,50.));
    gui.add(slider_tilt_c.setup("tilt_c",0.,-50.,50.));
    gui.add(slider_pan_b.setup("pan_b",0.,-50.,50.));
    

    
}

//--------------------------------------------------------------
void ofApp::update() {
    
    
    
    while(receiver.hasWaitingMessages()){
        
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        //AbletonからのOSCを取得
        if(m.getAddress() == "/ableton/pan_a"){
            
            pan_a_oscData = m.getArgAsFloat(0);
            
            //cout << "osc :" << ofToString(pan_a_oscData)  <<endl;
        }
        else if(m.getAddress() == "/ableton/tilt_a"){
            
            tilt_a_oscData = m.getArgAsFloat(0);
            
            //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
        }
        else if(m.getAddress() == "/ableton/tilt_b"){
            
            tilt_b_oscData = m.getArgAsFloat(0);
            
            //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
        }


    }
	 
    
    world.update();
   
    //ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    // store the position of the ground //
    ofVec3f pos = ground.getPosition();
    
    arm.setOsc(pan_a_oscData, tilt_a_oscData, tilt_b_oscData);
    //arm.setOsc(slider_pan_a*0.1, slider_tilt_a*0.1, slider_tilt_b*0.1);
    arm.update();

}

//--------------------------------------------------------------
void ofApp::draw() {
	glEnable( GL_DEPTH_TEST );
	//camera.begin();
    easyCam.begin();
	
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
    
    arm.draw();

    
    //ofSetColor(100,100,100);
    //ground.draw();
    
    easyCam.end();
    
    glDisable( GL_DEPTH_TEST );
    
    gui.draw();
    
    ground_colliding = FALSE;
}

//--------------------------------------------------------------


void ofApp::onCollision(ofxBulletCollisionData& cdata) {
    
    if(ground == cdata){
        
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
