#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate(60);
	ofBackground(128);
	ofEnableAlphaBlending();

	ofDisableArbTex();

	MEDIA_LOAD_TEXTURE("smoke2.png", TEX_SMOKE);
	MEDIA_GET_TEXURE(TEX_SMOKE)->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	MEDIA_LOAD_TEXTURE("fire.png", TEX_FIRE);
	MEDIA_GET_TEXURE(TEX_FIRE)->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	MEDIA_LOAD_TEXTURE("explosion.png", TEX_EXPLOSION);
	MEDIA_GET_TEXURE(TEX_EXPLOSION)->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);


	e.setup( MEDIA_GET_TEXURE(TEX_SMOKE), MEDIA_GET_TEXURE(TEX_FIRE), MEDIA_GET_TEXURE(TEX_EXPLOSION) );
	e.setSmokeProperties(
							5,					//random spawn pos offset from debris
							ofVec2f(0,-50),	//smoke particles speed in pix / sec
							ofVec2f(50,0),	//smoke particles acc in pix / sec
							0.9					//life in seconds
						 );

//  GUI.setup("my sample gui", 300, 600);
//
//	GUI.addSlider("strength", 10, 2900, 1200, &strength);
//	GUI.addSlider("numP", 5, 50, 15, &numP);
//	GUI.addSlider("friction", 0.8, 1, 0.93, &friction);
//	GUI.addSlider("debris life", 0.2, 1.7, 0.9, &life);
//	GUI.addSlider("object radius", 0, 50, 0, &offset);
//	GUI.addSlider("spawnOffset", 0, 50, 5, &spawnOffset);
//	GUI.addSlider("smokeLife", 0.2, 4, 0.9, &smokeLife);
//	GUI.addSlider("gravity", 0, 1000, 200, &gravity);

	strength = 1200;
	numP = 15;
	friction = 0.93;
	life = 0.9;
	offset = 0;
	spawnOffset = 5;
	smokeLife = 0.9;
	gravity = 200;


//	OFX_REMOTEUI_SERVER_SETUP();
//
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(strength, 10, 2900);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(numP, 5, 50);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(friction, 0.8, 1);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(life, 0.2, 1.7);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(offset, 0, 50);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(spawnOffset, 0, 50);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(smokeLife, 0.2, 4);
//	OFX_REMOTEUI_SERVER_SHARE_PARAM(gravity, -1000, 1000);
}


//--------------------------------------------------------------
void testApp::update(){

	//if (ofGetFrameNum() == 2) GUI.show();

	e.setSmokeProperties(
						 spawnOffset,					//random spawn pos offset from debris
						 ofVec2f(0,-50),	//smoke particles speed in pix / sec
						 ofVec2f(50,0),	//smoke particles acc in pix / sec
						 smokeLife					//life in seconds
						 );

	e.update(1./60.);
	if( ofGetFrameNum()%10 == 0) ofSetWindowTitle( ofToString(ofGetFrameRate()));
	//OFX_REMOTEUI_SERVER_UPDATE(0.016666f);
}

//--------------------------------------------------------------
void testApp::draw(){

	e.drawMesh();
	//e.drawDebug();
	ofSetColor(250,0,0);
	ofDrawBitmapString("left mouse button to start explosion, right mouse button to stop post-smoke", 20,20);
}


//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	if (button == 0){
		e.explode(	ofVec2f(x,y),
				  strength,	//explosion strength
				  numP,		//num debris particles
				  friction,	//air friction
				  ofVec2f(0, gravity), // gravity vector
				  life,		//particles life duration
				  offset		//initial position randomness, should be exploded object radius +-
				  );

	}else{
		e.stopExplosion();
	}

}

