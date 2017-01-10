#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//ofSetFrameRate(60);
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
    
    for(int i=0; i<NUM_OF_ARM; i++){
        for(int j=0; j<MUSICTIMEMILLIS; j++){
            mData[i].pan_a[j] = -1.;
            mData[i].tilt_a[j] = -1.;
            mData[i].tilt_b[j] = -1.;
            mData[i].tilt_c[j] = -1.;
            mData[i].pan_b[j] = -1.;
        }
    }
    //ファイル作成
    for(int i=0; i<NUM_OF_ARM; i++){
        string filePath = "motionData_arm" + ofToString(i) + ".csv";
        ofFile file(filePath,ofFile::WriteOnly);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    
    elapsedTime = ofGetElapsedTimeMillis();
    
    
    //OSCのやりとり
    while(receiver.hasWaitingMessages()){
        
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        for(int i=0; i<NUM_OF_ARM; i++){
            
            if(startMusicFlg == FALSE){
                //メッセージが来たら
                //if(!receiver.hasWaitingMessages()){
                    startMusicTime = elapsedTime;
                    startMusicFlg = TRUE;
                    cout << "START MUSIC : " << ofToString(startMusicTime)  <<endl;
                //}
            }
        
        //AbletonからのOSCを取得
            if(m.getAddress() == "/arm_"+ofToString(i)+"/pan_a"){
            
                osc[i].pan_a = m.getArgAsFloat(0);
                slider_pan_a[i] = osc[i].pan_a;
            
                //cout << "osc :" << ofToString(pan_a_oscData)  <<endl;
            }
            else if(m.getAddress() == "/arm_"+ofToString(i)+"/tilt_a"){
            
                osc[i].tilt_a = m.getArgAsFloat(0);
                slider_tilt_a[i] = osc[i].tilt_a;
            
                //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
            }
            else if(m.getAddress() == "/arm_"+ofToString(i)+"/tilt_b"){
            
                osc[i].tilt_b = m.getArgAsFloat(0);
                slider_tilt_b[i] = osc[i].tilt_b;
            
                //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
            }
            else if(m.getAddress() == "/arm_"+ofToString(i)+"/tilt_c"){
                
                osc[i].tilt_c = m.getArgAsFloat(0);
                slider_tilt_c[i] = osc[i].tilt_c;
                
                //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
            }
            else if(m.getAddress() == "/arm_"+ofToString(i)+"/pan_b"){
                
                osc[i].pan_b = m.getArgAsFloat(0);
                slider_pan_b[i] = osc[i].pan_b;
                
                //cout << "osc :" << ofToString(pan_b_oscData)  <<endl;
            }
        }
    }
    
    //モーションデータ(60fps)記録
    if(startMusicFlg == TRUE){
        
        musicTime = elapsedTime - startMusicTime;
        
        for(int i=0; i<NUM_OF_ARM; i++){
            
            cout << "writing motion data... time : " << ofToString(musicTime)  <<endl;
            mData[i].pan_a[musicTime] = osc[i].pan_a;
            mData[i].tilt_a[musicTime] = osc[i].tilt_a;
            mData[i].tilt_b[musicTime] = osc[i].tilt_b;
            mData[i].tilt_c[musicTime] = osc[i].tilt_c;
            mData[i].pan_b[musicTime] = osc[i].pan_b;
            
        }
    }
    
    
    
    world.update();
   
    //ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    // store the position of the ground //
    ofVec3f pos = ground.getPosition();
    for(int i=0; i<NUM_OF_ARM; i++){
        arm[i].setOsc(osc[i].pan_a, osc[i].tilt_a, osc[i].tilt_b, osc[i].tilt_c, osc[i].pan_b);
        //arm[i].setOsc(slider_pan_a[i], slider_tilt_a[i], slider_tilt_b[i], slider_tilt_c[i], slider_pan_b[i]);
        arm[i].update();
    }
    
    preFrame = -1;

}

//--------------------------------------------------------------
void ofApp::draw() {
	glEnable( GL_DEPTH_TEST );
	//camera.begin();
    easyCam.begin();
	
	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	world.drawDebug();
	
    
    if(colliding == TRUE){
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
    
    colliding = FALSE;
    
    stringstream ss;
    ss << "framerate: " << ofToString(ofGetFrameRate(), 0);
    ofDrawBitmapString(ss.str(), 10, 20);
}

//--------------------------------------------------------------


void ofApp::onCollision(ofxBulletCollisionData& cdata) {
    
    if(ground == cdata){
        
        colliding = TRUE;
    }
    
    for(int i=0;i<NUM_OF_ARM;i++){
        if(*arm[i].tilt_a == cdata){
            if(*arm[i].pan_a != cdata && *arm[i].tilt_b != cdata){
                colliding = TRUE;
            }
        }
        else if(*arm[i].tilt_b == cdata){
            if(*arm[i].tilt_a != cdata && *arm[i].tilt_c != cdata){
                colliding = TRUE;
            }
        }
        else if(*arm[i].tilt_c == cdata){
            if(*arm[i].tilt_b != cdata && *arm[i].pan_b != cdata){
                colliding = TRUE;
            }
        }
        else if(*arm[i].pan_b == cdata){
            if(*arm[i].tilt_c != cdata){
               colliding = TRUE;
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    if(key == 's'){
        
        //線形補間
        for(int i=0; i<NUM_OF_ARM; i++){
            for(int j=0; j<MUSICTIMEMILLIS; j++){
                if(mData[i].pan_a[j] != -1){
                    if(preFrame == -1){
                        preFrame = j;
                    }
                    
                    else{
                        long frame;
                        frame = j;
                        
                        for(int m = preFrame+1; m<frame; m++){
                            mData[i].pan_a[m] =  mData[i].pan_a[preFrame]+(mData[i].pan_a[frame]-mData[i].pan_a[preFrame])*(m-preFrame)/(frame-preFrame);
                            mData[i].tilt_a[m] = mData[i].tilt_a[preFrame]+(mData[i].tilt_a[frame]-mData[i].tilt_a[preFrame])*(m-preFrame)/(frame-preFrame);
                            mData[i].tilt_b[m] = mData[i].tilt_b[preFrame]+(mData[i].tilt_b[frame]-mData[i].tilt_b[preFrame])*(m-preFrame)/(frame-preFrame);
                            mData[i].tilt_c[m] = mData[i].tilt_c[preFrame]+(mData[i].tilt_c[frame]-mData[i].tilt_c[preFrame])*(m-preFrame)/(frame-preFrame);
                            mData[i].pan_b[m] =  mData[i].pan_b[preFrame]+(mData[i].pan_b[frame]-mData[i].pan_b[preFrame])*(m-preFrame)/(frame-preFrame);
                        }
                        preFrame = frame;
                    }
                }
            }
        }
        
        
        //すべてをデータで書き出す
        for(int i=0; i<NUM_OF_ARM; i++){
            
            string filePath = "motionData_arm" + ofToString(i) + ".csv";
            ofFile file(filePath,ofFile::WriteOnly);
            
            for(int j=0; j<MUSICTIMEMILLIS; j++){
        
                file << ofToString(j) + ",";
                file << ofToString(mData[i].pan_a[j]) + ",";
                file << ofToString(mData[i].tilt_a[j]) + ",";
                file << ofToString(mData[i].tilt_b[j]) + ",";
                file << ofToString(mData[i].tilt_c[j]) + ",";
                file << ofToString(mData[i].pan_b[j]) + "\n";
        
            }
            
            file.close();
            
        }
    }
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
