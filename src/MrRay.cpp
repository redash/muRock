#include "MrRay.h"

MrRay::MrRay():r0(0),v(0){Init();};

MrRay::MrRay(const Vect &R0, const Vect &V):r0(R0),v(V){
	Init();
};

MrRay::MrRay(const Vec3 &R0, const Vec3 &V):r0(R0,1),v(V,0){
	Init();
};

MrRay::~MrRay(){Clear();}

Vect MrRay::R(double t){
	return Vect(r0[0]+v[0]*t,r0[1]+v[1]*t,r0[2]+v[2]*t,1);
}

void MrRay::Init(){
	cpoints.reserve(NP_MAX);
	r0[3]=1; v[3]=0;
}