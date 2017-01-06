#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "btGhostObject.h"
#include "MPArm.h"

#define PORT 2346
#define NUM_OF_ARM 2

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

    bool ground_colliding;
    
    MPArm arm[NUM_OF_ARM];
    
    ///TestData boxdata;
    
private:
    ofxOscReceiver receiver;
    float pan_a_oscData;
    float tilt_a_oscData;
    float tilt_b_oscData;
    float tilt_c_oscData;
    float pan_b_oscData;
    
};
