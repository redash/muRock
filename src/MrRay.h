#ifndef MR_RAY__H
#define MR_RAY__H 

#include "basic_types.h"
#include <vector>

class CrossPoint
{
public:
	CrossPoint(int Id, float nv, float T):nid(Id),t(T){if(nv<0)nid=-nid;};
	CrossPoint(int nId, float T):nid(nId),t(T){}
	bool In() {return  nid<0;  } //true if this point is IN
	bool Out(){return  nid>0;  } //true if this point is OUT
	unsigned ShapeId() {return abs(nid);} //get shape Id

	//comparison operators for sorting
	inline bool operator > (const CrossPoint &cp) const {return (t> cp.t);}
	inline bool operator < (const CrossPoint &cp) const {return (t< cp.t);}
	inline bool operator >=(const CrossPoint &cp) const {return (t>=cp.t);}
	inline bool operator <=(const CrossPoint &cp) const {return (t<=cp.t);}
public:
	int nid;   //crossing shape ID,multiplied by sign((n*v))
	float t;  //crossing "time"
};
//----------------------------------------------------
class MrRay
{
	const unsigned NP_MAX=20;
public:
	MrRay();
	MrRay(const Vect &R0, const Vect &V);
	MrRay(const Vec3 &R0, const Vec3 &V);
	~MrRay();

	//propagate to time t
	Vect  R(float t);
	float X(float t);
	float Y(float t);
	float Z(float t);
	//getters
	Vect &V() {return v;}
	float V_2() {return v [0]*v [0]+v [1]*v [1]+v [2]*v [2];}
	float R0_2(){return r0[0]*r0[0]+r0[1]*r0[1]+r0[2]*r0[2];}
	float R0_V(){return r0[0]*v [0]+r0[1]*v [1]+r0[2]*v [2];}
	Vect &R0(){return r0;}

	void AddCrossPoint(const CrossPoint & cp){cpoints.push_back(cp);}

	void Init();
	void Clear(){cpoints.clear();}

public:
	std::vector<CrossPoint> cpoints;
	Vect r0,v;
};

#endif
