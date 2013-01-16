//
//  ExplosionSmoke.h
//  emptyExample
//

#pragma once

#include "ofMain.h"
#include "ofxMediaDB.h"

#ifndef PIXEL_SCALE //define this in your code to override
#define PIXEL_SCALE 4
#endif


typedef struct{
	ofVec2f p;
	float originalLife;
	float life;
	ofVec2f s;
	ofVec2f acc;
	ofColor color;
	float friction;
}ExplosionSmokeParticle;


class ExplosionSmoke{
	
	public:

	~ExplosionSmoke(){
		reset();
	}


	void setup(){
		fadingOut = 1.0;
		//c = 0;
	}


	void update( float dt){

		for(int i = smokeP.size()-1; i >= 0 ; i--){
			smokeP[i]->s *= smokeP[i]->friction;
			smokeP[i]->p += dt * smokeP[i]->s;
			smokeP[i]->life -= dt;
			smokeP[i]->s += smokeP[i]->acc * dt;
			if (smokeP[i]->life <= 0.0f){
				free (smokeP[i]);
				smokeP.erase(smokeP.begin() + i);
				i--;
			}
		}
	}


	bool isFinished(){ return smokeP.size() == 0; }

	
	void reset(){
		for(int i = smokeP.size()-1; i >= 0 ; i--){
			free (smokeP[i]);
			smokeP.erase(smokeP.begin() + i);
		}
	}
	

	void fillMesh( ofMesh & mesh){

		for(int i = 0; i < smokeP.size() ; i++){
			float percent = ( smokeP[i]->life / smokeP[i]->originalLife);
			if( percent < 0.0f ) percent = 0.0f;
			float s = 0.5f + 0.5f * ( 1.0f - percent );
			//int c = (1.0f - smokeP[i]->darkness) * 255.0f;
			//printf("%f %f\n", percent, fadingOut);
			ofColor c = smokeP[i]->color;
			c.a = 255.f * percent * fadingOut;
			mesh.addColor( c );
			mesh.addVertex( smokeP[i]->p );
		}
	}

	void spawnSmoke(ofVec2f pos, float randomOffset = 15, ofColor c = ofColor(255), ofVec2f vel = ofVec2f(0,0), ofVec2f acc = ofVec2f(0,0), float life_ = 0.35, float friction = 1.0f) {

		//if ( particles.size() < MAX_SMOKE_PARTICLES ){
		//if (c%2 == 1){
			ExplosionSmokeParticle *p = (ExplosionSmokeParticle*) malloc( sizeof(ExplosionSmokeParticle) );
			p->originalLife = life_ * ofRandom(0.55, 1.25);
			p->life = p->originalLife ;
			p->friction = friction;
			p->acc = acc * ofRandom(0.75f, 1.25f);
			p->s = vel * ofRandom(0.75f, 1.25f);
			p->p = pos;
			p->color = c * ofRandom(0.75, 1.0);// = ofClamp(darkness + ofRandom(-0.1, 0.1), 0, 1);
			p->p.x += ofRandom(-randomOffset, randomOffset);
			p->p.y += ofRandom(-randomOffset, randomOffset);
			smokeP.push_back(p);
		//}
		//c++;
	}


	float fadingOut; // controls fadeout alpha when near dead

private:

	//int c;
	float w, h;
	vector<ExplosionSmokeParticle*> smokeP;
	
};

