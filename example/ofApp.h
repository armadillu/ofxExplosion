#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "ofxTimeMeasurements.h"

#include "Explosion.h"

#define CONFIGS_DIRECTORY	"configs"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key){}
	void mouseMoved(int x, int y ){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button){}
	void windowResized(int w, int h){}


	// APP SETUP ////////////////////////////////////////////

	Explosion e;

	ofTexture * smokeTex;
	ofTexture * fireTex;
	ofTexture * explosionTex;

	float strength;
	int numP;
	float friction;
	float life;
	float offset;
	float spawnOffset;
	float smokeLife;
	float gravity;
	int pixelScale;

	ofTexture bg;


};
