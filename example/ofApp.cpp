#include "ofApp.h"


void ofApp::setup(){

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofBackground(22);

	ofDisableArbTex();

	ofLoadImage(bg, "images/unsquadron.png");
	bg.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	smokeTex = new ofTexture();
	ofLoadImage(*smokeTex, "images/smoke2.png");
	smokeTex->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	fireTex = new ofTexture();
	ofLoadImage(*fireTex, "images/fire.png");
	fireTex->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	explosionTex = new ofTexture();
	ofLoadImage(*explosionTex, "images/explosion.png");
	explosionTex->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	e.setup( smokeTex, fireTex, explosionTex );
	
	ofEnableArbTex();


	strength = 950;
	numP = 16;
	friction = 0.93;
	life = 0.99;
	offset = 0;
	spawnOffset = 4;
	smokeLife = 1.6;
	gravity = 100;
	pixelScale = 3;

	RUI_SET_CONFIGS_DIR(CONFIGS_DIRECTORY);
	RUI_SETUP();

	RUI_NEW_GROUP("EXPLOSION PARAMS");
	RUI_SHARE_PARAM(strength, 10, 2900);
	RUI_SHARE_PARAM(numP, 5, 50);
	RUI_SHARE_PARAM(friction, 0.8, 1);
	RUI_SHARE_PARAM(life, 0.2, 1.7);
	RUI_SHARE_PARAM(offset, 0, 50);
	RUI_SHARE_PARAM(spawnOffset, 0, 10);
	RUI_SHARE_PARAM(smokeLife, 0.2, 4);
	RUI_SHARE_PARAM(gravity, -1000, 1000);
	RUI_NEW_COLOR();
	RUI_SHARE_PARAM(pixelScale, 1, 10);

	RUI_LOAD_FROM_XML();

	TIME_SAMPLE_SET_FRAMERATE(60);
}


void ofApp::update(){

	float dt = 1.f/60.f;

	e.setPixelScale(pixelScale);
	e.setSmokeProperties(
						 spawnOffset,		//random spawn pos offset from debris
						 ofVec2f(0,-50),	//smoke particles speed in pix / sec
						 ofVec2f(50,0),		//smoke particles acc in pix / sec
						 smokeLife			//life in seconds
						 );

	e.update(dt);


}


void ofApp::draw(){

	ofSetColor(255);
	bg.draw(0,0,ofGetWidth(), ofGetHeight());

	e.drawMesh();

	ofSetColor(250,0,0);
	ofDrawBitmapString("left mouse button to start explosion, right mouse button to stop post-smoke", 20,20);

}


void ofApp::keyPressed(int key){

	e.stopPostExplosionSmoke();
}


void ofApp::mousePressed(int x, int y, int button){

	if (button == 0){
		e.explode(ofVec2f(x,y), //where
				  ofVec2f(0, 2 * M_PI), //explosion angle range x:min y:max [0..¹]
				  strength,	//explosion strength
				  numP,		//num debris particles
				  friction,	//air friction
				  ofVec2f(0, gravity), // gravity vector
				  life,		//particles life duration
				  offset		//initial position randomness, should be exploded object radius +-
				  );

	}else{
		e.reset();
	}

}

