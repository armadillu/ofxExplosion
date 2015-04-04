//
//  Dust.h
//  emptyExample
//

#pragma once

#include "ofMain.h"
#include "ExplosionParticles.h"
#include "ofxColorGradient.h"


class Explosion{
	
public:

	void setup(ofTexture * smokeTex_, ofTexture * debrisTex_, ofTexture * explosionTex_, ofxColorGradient smoke = ofxColorGradient(), ofxColorGradient fire = ofxColorGradient()){

		explosionTex = explosionTex_;
		smokeTex = smokeTex_;
		debrisTex = debrisTex_;
		running = false;
		exploding = 0.0f;
		pixelScale = 4.0;

		//if no gradient supplied, make a default one
		if (smoke.getNumColors() == 0){
			//default smoke color timeline
			smokeGradient.addColor(ofColor(128, 0));
			smokeGradient.addColor(ofColor(128, 255));
			smokeGradient.addColor(ofColor(90, 255));
			smokeGradient.addColor(ofColor(64, 100));
			smokeGradient.addColor(ofColor(32, 0));
		}else{
			smokeGradient = smoke;
		}

		//if no gradient supplied, make a default one
		if (fire.getNumColors() == 0){
			//define fire color timeline
			float gain = 0.5;
			fireGradient.addColor( ofColor(255,255,255, 120) * gain );
			fireGradient.addColor( ofColor(255,240,180, 100) * gain );
			fireGradient.addColor( ofColor(230,130,48, 100) * gain );
			fireGradient.addColor( ofColor(151,37,0, 100) * gain );
			fireGradient.addColor( ofColor(96,0,0, 90) * gain );
			//fireGradient.addColor( ofColor(160,140,120, 90) * gain );
			fireGradient.addColor( ofColor(40,24,8,90) * gain );
			fireGradient.addColor( ofColor(0,0,0,00) * gain );
		}else{
			fireGradient = fire;
		}

		mainSmoke.setup();
		smokeParticles.setup();

		mainSmokeMesh.setMode(OF_PRIMITIVE_POINTS);
		fireParticles.setup();
	}

	void setPixelScale(float s){
		pixelScale = s;
	};

	void setSmokeProperties( float spawnRandomOffset_, ofVec2f smokeSpeed_, ofVec2f smokeAcc_, float smokeLife_){
		spawnOffset = spawnRandomOffset_;
		smokeSpeed = smokeSpeed_;
		smokeAcc = smokeAcc_;
		smokeLife = smokeLife_;
	}


	void explode(const ofVec2f & pos, const ofVec2f & angleRange, float strength,
				 int numParticles = 10, float friction = 0.93f, const ofVec2f & gravity_ = ofVec2f(0,0),
				 float life = 1.2, float objectRadius = 0 ){

		//stopSpeed speed at which we consider stopped moving (this it stops emiting smoke), not taking in account gravity!
		running = true;
		explStrength = strength;
		gravity = gravity_;
		center = pos;
		explodinObjectRadius = objectRadius;
		exploding = 0.0001;
		running = true;

		smokeParticles.explode(pos, angleRange, numParticles, strength, life, friction, spawnOffset, smokeTex, fireGradient, smokeGradient);
		fireParticles.explode(pos, angleRange, numParticles, strength * 0.35f, life * 0.5f, friction, spawnOffset, smokeTex, fireGradient, fireGradient);

		for(int i = 0; i < 30; i++){ //at explode time, make a lot fo smoke in middle
			spawnInitialCenterSmoke();
		}

	}

	void spawnInitialCenterSmoke(){
		float l = explStrength * 0.33f;
		ofVec2f randomSpeed = ofVec2f( ofRandom(-l,l), ofRandom(-l,l) );
		mainSmoke.spawnSmoke(center, 0, smokeGradient.getColorAtPercent(exploding), randomSpeed, smokeAcc/*acc*/, smokeLife * 2.0f, 0.93f); //friction for the first cloud, so it stays around center
	}


	void reset(){
		exploding = 0.0f;
		running = false;
		smokeParticles.reset(); // the flying objects, start from center, run in all directions, spawn smoke and fire
		fireParticles.reset();
		mainSmoke.reset();
		mainSmokeMesh.clear();
	}
	

	void stopPostExplosionSmoke(){
		running = false;
	}


	void update( float dt ){

		if(exploding > 0.0f) exploding += dt;
		exploding = ofClamp(exploding, 0.0f, 1.0f);

		smokeParticles.update(dt, gravity, spawnOffset, smokeSpeed, smokeAcc, smokeLife);
		fireParticles.update(dt, gravity, spawnOffset, smokeSpeed * 0.5f, smokeAcc * 0.5f, smokeLife * 0.5f);

		if (running){ // the smoke in the center gets spawned here
			if(exploding < 0.2f){
				spawnInitialCenterSmoke();
			}
			float l = smokeSpeed.length() + 1;
			ofVec2f randomSpeed = ofVec2f( ofRandom(-l,l), ofRandom(-l,l) );
			mainSmoke.spawnSmoke(center, explodinObjectRadius, smokeGradient.getColorAtPercent(exploding), randomSpeed, smokeAcc/*acc*/, smokeLife * 2);
		}
		mainSmoke.update(dt);
		
		fillMesh(); // if using mesh mode!
	}


	void drawMesh(){

		//draw the fire trails udnerneath it all
//		glPointSize( PIXEL_SCALE * debrisTex->getWidth() );
//		debrisTex->bind();
//			//fireParticles.drawSmokeMesh();
//			smokeParticles.drawFireMesh();
//		debrisTex->unbind();

		ofEnablePointSprites();
		ofEnableAlphaBlending();

		//the smoke trails
		glPointSize( pixelScale * smokeTex->getWidth() );
		smokeTex->bind();
			mainSmokeMesh.draw();
			smokeParticles.drawSmokeMesh();
		smokeTex->unbind();

		ofEnableBlendMode(OF_BLENDMODE_ADD);

		glPointSize( pixelScale * explosionTex->getWidth() );
		explosionTex->bind();
		fireParticles.drawFireMesh();
		explosionTex->unbind();

		//finally, the projectiles
		glPointSize( pixelScale * debrisTex->getWidth() );
		debrisTex->bind();
			smokeParticles.drawFireMesh();
			fireParticles.drawFireMesh();
		debrisTex->unbind();

		ofDisablePointSprites();
		ofEnableAlphaBlending();
	}

	void drawDebug(){
		float h = 20.0f;
		float w = 150.0f;
		fireGradient.drawDebug(0,ofGetHeight(), w, -h);
		smokeGradient.drawDebug(0,ofGetHeight() -h, w, -h);
	}

	bool isExploding(){
		return exploding > 0.0f && exploding < 1.0f;
	}

	bool hasFinishedExploding(){
		return exploding >= 1.0f;
	}

private:

	void fillMesh(){
		mainSmokeMesh.clear();
		mainSmoke.fillMesh(mainSmokeMesh);
	}

	ofTexture * smokeTex;
	ofTexture * debrisTex;
	ofTexture * explosionTex;

	float explStrength;
	float spawnOffset;
	ofVec2f smokeSpeed;
	ofVec2f smokeAcc;
	float smokeLife;
	ofVec2f gravity;
	ofVec2f center;
	bool running;
	float explodinObjectRadius;
	float exploding; // [0..1], 0 means not exploding, 1 means exploded and over

	ofxColorGradient fireGradient;
	ofxColorGradient smokeGradient;

	ExplosionParticles smokeParticles; // the flying objects, start from center, run in all directions, spawn smoke and fire
	ExplosionParticles fireParticles;

	ExplosionSmoke mainSmoke; //this is the smoke that runs in in the explosion center
	ofMesh mainSmokeMesh;

	float pixelScale;
};

