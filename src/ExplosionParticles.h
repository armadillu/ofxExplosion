//
//  ExplosionParticles.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 09/01/13.
//
//

#ifndef emptyExample_ExplosionParticles_h
#define emptyExample_ExplosionParticles_h

#include "ofMain.h"
#include "ExplosionSmoke.h"
#include "ofxColorGradient.h"

class ExplosionParticle{

public:

	~ExplosionParticle(){
		delete smoke;
	};

	ofVec2f p;
	float originalLife;
	float life;
	ofVec2f s;
	ofTexture* tex;
	float friction;
	ExplosionSmoke * smoke;
	ExplosionSmoke * fire;
};


class ExplosionParticles{

public:

	void setup(){
		smokeMesh.setMode(OF_PRIMITIVE_POINTS);
		headMesh.setMode(OF_PRIMITIVE_POINTS);
		fireMesh.setMode(OF_PRIMITIVE_POINTS);
	}


	void update(float dt, ofVec2f & gravity, float explodinObjectRadius, ofVec2f  smokeSpeed, ofVec2f  smokeAcc, float smokeLife){

		for(int i = smokeParticles.size()-1; i >= 0 ; i--){

			float percent = ( smokeParticles[i]->life / smokeParticles[i]->originalLife );
			if (percent < 0.0f) percent = 0.0f;
			smokeParticles[i]->s = smokeParticles[i]->s * smokeParticles[i]->friction + gravity * dt;
			smokeParticles[i]->p += dt * smokeParticles[i]->s;
			smokeParticles[i]->life -= dt;

			if (smokeParticles[i]->life > 0.0){
				smokeParticles[i]->smoke->spawnSmoke(smokeParticles[i]->p, explodinObjectRadius, trailGradient.getColorAtPercent(1-percent), smokeSpeed, smokeAcc/*acc*/, smokeLife );
				smokeParticles[i]->fire->spawnSmoke(smokeParticles[i]->p, explodinObjectRadius * 2, headGradient.getColorAtPercent(1-percent), smokeSpeed, smokeAcc * 2/*acc*/, smokeLife * 0.5 );
			}

			smokeParticles[i]->smoke->update(dt);
			smokeParticles[i]->fire->update(dt);

			if (smokeParticles[i]->smoke->isFinished()){
				free (smokeParticles[i]);
				smokeParticles.erase(smokeParticles.begin() + i);
				i--;
			}
		}
		//printf("%d\n", smokeParticles.size());
		if(smokeParticles.size() > 0){
			fillMesh();
		}
	}


	void explode( ofVec2f pos, int numP, float strength, float life, float friction, float initialOffsetRange, ofTexture * smokeTex, ofxColorGradient headGradient_, ofxColorGradient trailsGradient_){

		trailGradient = trailsGradient_;
		headGradient = headGradient_;

		//printf("%f\n", initialOffsetRange );

		for(int i = 0; i < numP; i++ ){
			ExplosionParticle *p = (ExplosionParticle*) malloc( sizeof(ExplosionParticle) );
			float spe = strength;
			p->s.x = ofRandom(-spe, spe);
			p->s.y = ofRandom(-spe, spe);
			//			float s = p->s.length();
			//			float x = stopSpeed / s; // stop_speed = speed * friction ^ x  >> to calc x (# iterations til part stops), we do x = log(base friction)(stop_speed / speed);
			//			float numFramesTilStop = logf(x)/logf(friction); // logb (x) = (loga(x)) / (loga(B))
			p->originalLife = life; // num updates @ 60fps >> lifetime
			p->life = p->originalLife;
			p->p = pos;
			p->p.x += ofRandom(-initialOffsetRange, initialOffsetRange);
			p->p.y += ofRandom(-initialOffsetRange, initialOffsetRange);
			p->friction = friction;
			p->smoke = new ExplosionSmoke();
			p->smoke->setup();
			p->fire = new ExplosionSmoke();
			p->fire->setup();

			smokeParticles.push_back(p);
		}
	}

	void drawDebug(){
		ofSetRectMode(OF_RECTMODE_CENTER);
		for(int i = smokeParticles.size()-1; i >= 0 ; i--){
			ofSetColor(255,0,0);
			ofRect(smokeParticles[i]->p, PIXEL_SCALE, PIXEL_SCALE);
		}
		ofSetRectMode(OF_RECTMODE_CORNER);
	}

	void drawSmokeMesh(){
		smokeMesh.draw();
	}

	void drawFireMesh(){
		fireMesh.draw();
	}

	void drawHeadsMesh(){
		headMesh.draw();
	}

private:

	void fillMesh(){

		smokeMesh.clear();
		headMesh.clear();
		fireMesh.clear();

		for(int i = 0; i < smokeParticles.size(); i++ ){
			//smoke trails
			smokeParticles[i]->smoke->fillMesh( smokeMesh );
			smokeParticles[i]->fire->fillMesh( fireMesh );


			//head of smoke strains
			float percent = ( smokeParticles[i]->life / smokeParticles[i]->originalLife );
			if (percent < 0.0f) percent = 0.0f;
			headMesh.addColor( headGradient.getColorAtPercent(1.0f - percent));
			headMesh.addVertex(smokeParticles[i]->p);
		}
	}

	vector<ExplosionParticle*> smokeParticles;
	ofMesh headMesh;
	ofMesh smokeMesh;
	ofMesh fireMesh;
	ofxColorGradient trailGradient;
	ofxColorGradient headGradient;
	
};



#endif
