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
    
    for(int i=0; i<NUM_OF_ARM ; i++){
        arm[i].setup(world,0,500*i);
    }
	
    receiver.setup(PORT);
    
    //gui
    /*
    for(int i=0;i<NUM_OF_ARM;i++){
        gui[i].setup("panel%s",ofToString(i));
    }
    */
    
    for(int i=0;i<NUM_OF_ARM;i++){
        
        gui[i].setup("panel" + ofToString(i));
        
        gui[i].add(slider_pan_a[i].setup("pan_a" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_tilt_a[i].setup("tilt_a" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_tilt_b[i].setup("tilt_b" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_tilt_c[i].setup("tilt_c" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_pan_b[i].setup("pan_b" + ofToString(i),0.5,0.,1.));
    }
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
    
    
    while(receiver.hasWaitingMessages()){
        
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        //AbletonからのOSCを取得
        if(m.getAddress() == "/arm_1/pan_a"){
            
            pan_a_oscData = m.getArgAsFloat(0);
            
            //cout << "osc :" << ofToString(pan_a_oscData)  <<endl;
        }
        else if(m.getAddress() == "/arm_1/tilt_a"){
            
            tilt_a_oscData = m.getArgAsFloat(0);
            
            //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
        }
        else if(m.getAddress() == "/arm_1/tilt_b"){
            
            tilt_b_oscData = m.getArgAsFloat(0);
            
            //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
        }
    }
	 
    
    world.update();
   
    //ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    // store the position of the ground //
    ofVec3f pos = ground.getPosition();
    for(int i=0; i<NUM_OF_ARM; i++){
        //arm[i].setOsc(pan_a_oscData, tilt_a_oscData, tilt_b_oscData);
        arm[i].setOsc(slider_pan_a[i], slider_tilt_a[i], slider_tilt_b[i], slider_tilt_c[i], slider_pan_b[i]);
        arm[i].update();
    }

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
    
    for(int i=0; i<NUM_OF_ARM; i++){
        arm[i].draw();
    }
    
    //ofSetColor(100,100,100);
    //ground.draw();
    
    easyCam.end();
    
    glDisable( GL_DEPTH_TEST );
    
    for(int i=0; i<NUM_OF_ARM; i++){
        gui[i].draw();
    }
    
    ground_colliding = FALSE;
}

//--------------------------------------------------------------


void ofApp::onCollision(ofxBulletCollisionData& cdata) {
    
    if(ground == cdata){
        
        ground_colliding = TRUE;
    }
    
    for(int i=0;i<NUM_OF_ARM;i++){
        if(*arm[i].tilt_a == cdata){
            if(*arm[i].pan_a != cdata && *arm[i].tilt_b != cdata){
                cout << "ぶつかり" << ofToString(ofGetSeconds())<<endl;
            }
        }
        else if(*arm[i].tilt_b == cdata){
            
        }
        else if(*arm[i].tilt_c == cdata){
            
        }
        else if(*arm[i].pan_b == cdata){
            
        }
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
