#pragma once

#include "constants.h"

#define PIXEL_SCALE 4

#include "ofMain.h"
#include "Explosion.h"
#include "ofxMediaDB.h"
//#include "ofxNativeGuiAutoLayout.h"
//#include "ofxRemoteUI.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyReleased(int key);
		void mousePressed(int x, int y, int button);

		Explosion e;
		ofVboMesh m;

        //ofxNativeGuiAutoLayout GUI;
	float strength;
	float numP;
	float friction;
	float life;
	float offset;
	float spawnOffset;
	float smokeLife;
	float gravity;
};
