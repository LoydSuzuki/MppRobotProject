#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxGui.h"
#include "btGhostObject.h"

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
	
	ofxBulletWorldRigid			world;
	ofxBulletBox				ground;
    
    ofxBulletBox*                tilt_a;
    ofxBulletBox*                tilt_b;
    ofxBulletCustomShape*        tilt_as;
	
	ofxBulletSphere*			sphere;
	ofxBulletBox*				box;
	ofxBulletCone*				cone;
	ofxBulletCapsule*			capsule;
	ofxBulletCylinder*			cylinder;
	
	ofCamera					camera;
    
    ofxPanel gui;
    ofxFloatSlider angle_a;
    ofxFloatSlider angle_b;

    bool ground_colliding;
    
    struct TestData{
        int count;
        TestData():count(0){}
    };
    
    ///TestData boxdata;
    
};
