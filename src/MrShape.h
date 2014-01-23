#ifndef MRSHAPE__H
#define MRSHAPE__H

#include <cstring>
#include <iostream>
#include <map>
#include <glm/glm.hpp> 
//#include <glm/gtc/constants.hpp> 
#include <glm/gtc/matrix_transform.hpp>



typedef glm::vec3 Vec3;
typedef glm::vec4 Vect;
typedef glm::mat4 Matr;
std::ostream& operator<<(std::ostream& out, const Vec3& v); // output
std::ostream& operator<<(std::ostream& out, const Vect& v); // output
std::ostream& operator<<(std::ostream& out, const Matr& m); // output


//------------------------------------------------------
class MrObject
{
	//basic class for any object in scene
public:
	MrObject(uint Id=0);
	virtual ~MrObject(){};
	// setters&getters
	Vec3& Pos(){return pos;}
	Vec3& Ang(){return ang;}
	Vec3& Scl(){return scl;}
	//virtual functions for updating coordinates
	virtual void OnUpdatePos(){};
	virtual void OnUpdateAng(){};
	virtual void OnUpdateScl(){};
//text dumps
	virtual void Print();
protected:
	uint id;
	Vec3 pos;  // object center position vector  
	Vec3 ang;  // object angles vector
	Vec3 scl;  // object scale vector
};
//-----------------------------------------------------

class MrShape: public MrObject{
	//abstract class
public:
	static std::map<uint,MrShape> ShapesList;
public:
	MrShape();
	virtual ~MrShape();

	void OnUpdatePos(){};
	void OnUpdateAng(){};
	void OnUpdateScl(){};
	void UpdMatrix();
	// transformations
	inline Vect l2g(const Vect& v){return M_l2g*v;}
	inline Vect g2l(const Vect& v){return M_g2l*v;}
	// intersection checks
	//virtual double Intersect(const Vect &v,Vect &ipoint,Vect &inormal);
	
	//some virtual functions: 

	//for output
	virtual void Print();
	//for calculations in LOCAL frame
	virtual double Volume_LOC(){return 0;}
	virtual bool IsInside_LOC(Vect v){return false;}

	//global frame calculations
	double Volume(){return Volume_LOC()*scl[0]*scl[1]*scl[2];}
	bool IsInside(Vect v){return IsInside_LOC(g2l(v));}
	
public:
	Matr M_g2l;	//transformation matrix: Global to Local
	Matr M_l2g;	//transformation matrix: Global to Local
};
//-----------------------------------------------------
class MrBox:public MrShape
{
public:
	 MrBox(){;};
	~MrBox(){;};
	
	//some virtual functions: 
	//for output
	virtual void Print();
	//for calculations in LOCAL frame
	virtual double Volume_LOC();
	virtual bool IsInside_LOC(Vect v);
};
//-----------------------------------------------------
class MrSphere:public MrShape
{
public:
	 MrSphere(){;};
	~MrSphere(){;};
	
	//some virtual functions: 
	//for output
	virtual void Print();
	//for calculations in LOCAL frame
	virtual double Volume_LOC();
	virtual bool IsInside_LOC(Vect v);
};

#endif