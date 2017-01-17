#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "btGhostObject.h"
#include "MPArm.h"
#include "CubicSpline.h"

#define PORT 2346
#define NUM_OF_ARM 2
#define MUSICTIMEMILLIS 30000

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void onCollision(ofxBulletCollisionData& cdata);
    //bool ContactCallback( btManifoldPoint& manifold, void* object0, void* object1 );
    
    ofEasyCam                   easyCam;
	
	ofxBulletWorldRigid			world;
	ofxBulletBox				ground;
    
    ofxPanel gui[NUM_OF_ARM];
    ofxFloatSlider slider_pan_a[NUM_OF_ARM];
    ofxFloatSlider slider_tilt_a[NUM_OF_ARM];
    ofxFloatSlider slider_tilt_b[NUM_OF_ARM];
    ofxFloatSlider slider_tilt_c[NUM_OF_ARM];
    ofxFloatSlider slider_pan_b[NUM_OF_ARM];

    bool colliding;
    
    MPArm arm[NUM_OF_ARM];
    
    ///TestData boxdata;
    
private:
    ofxOscReceiver receiver;
    /*
    float pan_a_oscData[NUM_OF_ARM];
    float tilt_a_oscData[NUM_OF_ARM];
    float tilt_b_oscData[NUM_OF_ARM];
    float tilt_c_oscData[NUM_OF_ARM];
    float pan_b_oscData[NUM_OF_ARM];
    */
    
    struct oscFromAbleton{
        float pan_a;
        float tilt_a;
        float tilt_b;
        float tilt_c;
        float pan_b;
        
        float pre_pan_a;
        float pre_pan_b;
    };
    
    struct motionData{
        vector<double> mTime;
        vector<double> pan_a;
        vector<double> tilt_a;
        vector<double> tilt_b;
        vector<double> tilt_c;
        vector<double> pan_b;
    };
    
    int pan_a_rotation_num;
    int pan_b_rotation_num;
    
    oscFromAbleton osc[NUM_OF_ARM];
    motionData mData[NUM_OF_ARM];
    motionData mDataLowFPS[NUM_OF_ARM];
    motionData mDataSpline[NUM_OF_ARM];
    unsigned long elapsedTime;
    unsigned long startMusicTime;
    bool startMusicFlg;
    unsigned long musicTime;
    
    long preFrame;
    long musicFrame;
    
    
};
