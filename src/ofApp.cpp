#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
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
        arm[i].setup(world,0,500*i,90*i);
    }
	
    //osc receiver セットアップ
    receiver.setup(PORT);
    
    //gui
    /*
    for(int i=0;i<NUM_OF_ARM;i++){
        gui[i].setup("panel%s",ofToString(i));
    }
    */
    
    //ofxGUIセットアップ
    for(int i=0;i<NUM_OF_ARM;i++){
        
        gui[i].setup("panel" + ofToString(i));
        
        gui[i].add(slider_pan_a[i].setup("pan_a" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_tilt_a[i].setup("tilt_a" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_tilt_b[i].setup("tilt_b" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_tilt_c[i].setup("tilt_c" + ofToString(i),0.5,0.,1.));
        gui[i].add(slider_pan_b[i].setup("pan_b" + ofToString(i),0.5,0.,1.));
        
    }
    
    //ファイル作成
    for(int i=0; i<NUM_OF_ARM; i++){
        
        string filePath = "motionData_spline_arm" + ofToString(i) + ".csv";
        ofFile file(filePath,ofFile::WriteOnly);
        
    }
    
    //その他初期化
    musicFrame = 0;
    pan_a_rotation_num = 0;
    pan_b_rotation_num = 0;
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
    
    //armにAbletonからのデータを反映させる
    ofVec3f pos = ground.getPosition();
    for(int i=0; i<NUM_OF_ARM; i++){
        arm[i].setOsc(osc[i].pan_a, osc[i].tilt_a, osc[i].tilt_b, osc[i].tilt_c, osc[i].pan_b);
        //arm[i].setOsc(slider_pan_a[i], slider_tilt_a[i], slider_tilt_b[i], slider_tilt_c[i], slider_pan_b[i]);
        arm[i].update();
    }

    //モーションデータ(30fps)をvectorに記録
    if(startMusicFlg == TRUE){
        
        musicTime = elapsedTime - startMusicTime;
        
        for(int i=0; i<NUM_OF_ARM; i++){
            
            cout << "writing motion data... time : " << ofToString(musicTime)  <<endl;
            
            //pan oscを検証
            if(mData[i].pan_a.size()>1){
                
                //pan_a 回転数　インクリメント
                if(osc[i].pre_pan_a - osc[i].pan_a >= 0.9){
                    
                    pan_a_rotation_num ++ ;
                    
                }
                
                // pan_a 回転数 デクリメント
                else if(osc[i].pre_pan_a - osc[i].pan_a <= -0.9){
                    
                    pan_a_rotation_num -- ;
                    
                }
                
                //pan_b 回転数　インクリメント
                if(osc[i].pre_pan_b - osc[i].pan_b >= 0.9){
                    
                    pan_b_rotation_num ++ ;
                    
                }
                
                // pan_b 回転数 デクリメント
                else if(osc[i].pre_pan_b - osc[i].pan_b <= -0.9){
                    
                    pan_b_rotation_num -- ;
                    
                }
            }

            //モーションデータを記録
            mData[i].mTime.push_back(musicTime);
            mData[i].pan_a.push_back(osc[i].pan_a + pan_a_rotation_num);
            mData[i].tilt_a.push_back(osc[i].tilt_a);
            mData[i].tilt_b.push_back(osc[i].tilt_b);
            mData[i].tilt_c.push_back(osc[i].tilt_c);
            mData[i].pan_b.push_back(osc[i].pan_b + pan_b_rotation_num);
            
            //osc pre_panに現在のpanを代入
            osc[i].pre_pan_a = osc[i].pan_a;
            osc[i].pre_pan_b = osc[i].pan_b;
            
        }
        
        musicFrame++;
    }
    
    
    
    world.update();
   
    //ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    
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
    
    /*
    if(key == 'm'){
        
        
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
            
            string filePath = "motionData_1000fps_arm" + ofToString(i) + ".csv";
            ofFile file(filePath,ofFile::WriteOnly);
            
            for(int j=0; j<MUSICTIMEMILLIS; j++){
        
     
                file << ofToString(mData[i].mTime) + ",";
                file << ofToString(mData[i].pan_a[j]) + ",";
                file << ofToString(mData[i].tilt_a[j]) + ",";
                file << ofToString(mData[i].tilt_b[j]) + ",";
                file << ofToString(mData[i].tilt_c[j]) + ",";
                file << ofToString(mData[i].pan_b[j]) + "\n";
     
        
            }
            
            file.close();
            
        }
    }
    */
    
    //フレーム毎の数値を書き出す
    if(key == 'f'){
        
        for(int i=0; i<NUM_OF_ARM; i++){
        
            string filePath = "motionData_lowfps_arm" + ofToString(i) + ".csv";
            ofFile file(filePath,ofFile::WriteOnly);
            
            for(int j=0; j<mData[i].pan_a.size(); j++){
                
                file << ofToString(mData[i].mTime.at(j)) + ",";
                file << ofToString(mData[i].pan_a.at(j)) + ",";
                file << ofToString(mData[i].tilt_a.at(j)) + ",";
                file << ofToString(mData[i].tilt_b.at(j)) + ",";
                file << ofToString(mData[i].tilt_c.at(j)) + ",";
                file << ofToString(mData[i].pan_b.at(j)) + "\n";
                }
            
            file.close();
        }
    }
    
    //スプライン変換したデータを書き出す
    if(key == 's'){
        for(int i=0; i<NUM_OF_ARM; i++){
            CubicSpline *cs = new CubicSpline(mData[i].pan_a);
            string filePath = "motionData_spline_arm" + ofToString(i) + ".csv";
            ofFile file(filePath,ofFile::WriteOnly);
            
            /*
            for(double j=0; j<mData[i].mTime.size(); j+=0.030303){
                
                double tMillis = j*0.030303;
                mDataSpline[i].mTime.push_back(tMillis);
                mDataSpline[i].pan_a.push_back(cs->Calc(j));
            }
            */
            for(double j=0; j<mData[i].mTime.size()*33.3333; j++){
                
                mDataSpline[i].mTime.push_back(mData[i].mTime.at(j)/33.3333);
                mDataSpline[i].pan_a.push_back(cs->Calc(j/33.3333));
                mDataSpline[i].tilt_a.push_back(cs->Calc(j/33.3333));
                mDataSpline[i].tilt_b.push_back(cs->Calc(j/33.3333));
                mDataSpline[i].tilt_c.push_back(cs->Calc(j/33.3333));
                mDataSpline[i].pan_b.push_back(cs->Calc(j/33.3333));
            }
            for(int j=0; j<mDataSpline[i].mTime.size(); j++){
                file << ofToString(mDataSpline[i].mTime.at(j)) + ",";
                file << ofToString(mDataSpline[i].pan_a.at(j)) + ",";
                file << ofToString(mDataSpline[i].tilt_a.at(j)) + ",";
                file << ofToString(mDataSpline[i].tilt_b.at(j)) + ",";
                file << ofToString(mDataSpline[i].tilt_c.at(j)) + ",";
                file << ofToString(mDataSpline[i].pan_b.at(j)) + "\n";
            }
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
