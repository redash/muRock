#include "MrRay.h"

MrRay::MrRay():r0(0),v(0){Init();};

MrRay::MrRay(const Vect &R0, const Vect &V):r0(R0),v(V){
	Init();
};

MrRay::MrRay(const Vec3 &R0, const Vec3 &V):r0(R0,1),v(V,0){
	Init();
};

MrRay::~MrRay(){Clear();}

Vect MrRay::R(float t){return Vect(X(t),Y(t),Z(t),1);}

float MrRay::X(float t){return r0[0]+v[0]*t;}
float MrRay::Y(float t){return r0[1]+v[1]*t;}
float MrRay::Z(float t){return r0[2]+v[2]*t;}

void MrRay::Init(){
	cpoints.reserve(NP_MAX);
	r0[3]=1; v[3]=0;
}