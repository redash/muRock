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

inline void RotateM(Matr M,float ang,Vec3 axis){M=glm::rotate(M,ang,axis);}
inline void ScaleM (Matr M, Vec3 scl){M=glm::scale (M,scl);}
inline void TranslateM(Matr M,Vec3 shift){M=glm::translate(M,shift);}
inline float TraceM(Matr M){return M[0][0]*M[1][1]*M[2][2]*M[3][3];}

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
	const Vec3& Pos(){return pos;}
	const Vec3& Ang(){return ang;}
	const Vec3& Scl(){return scl;}
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

class MrShape{
	//abstract class
public:
	static std::map<uint,MrShape> ShapesList;
public:
	
	MrShape();
	MrShape(const MrObject &o);
	MrShape(Vec3  scl,Vec3  ang,Vec3  pos);
	//MrShape(Vec3& pos,Vec3& ang,Vec3& scl);
	virtual ~MrShape();

	//changing transformation matrices
	void InitM(); //set matrices to I
	void RotateM(float angle,const Vec3& axis); //apply rotation
	void TranslateM(const Vec3& shift); //apply translation
	void ScaleM(const Vec3& scale); //apply scaling
	//apply scaling,rotation and shift
	void UpdateM(const Vec3& pos,const Vec3& ang,const Vec3& scl);
	void UpdateM(MrObject &o){UpdateM(o.Pos(),o.Ang(),o.Scl());}
	// transformations
	inline Vect l2g(const Vect& v){return M_l2g*v;}
	inline Vect g2l(const Vect& v){return M_g2l*v;}
	
	// intersection checks
	//virtual double Intersect(const Vect &v,Vect &ipoint,Vect &inormal);
	
	//some virtual functions: 
	virtual const char* ShapeType(){return "Dummy";}
	//for output
	virtual void Print();
	//for calculations in LOCAL frame
	virtual double Volume_LOC(){return 0;}
	virtual bool IsInside_LOC(Vect v){return false;}
	//virtual Intersect_LOC();
	//global frame calculations
	double Volume(){return Volume_LOC()*TraceM(M_l2g);;}
	bool IsInside(Vect v){return IsInside_LOC(g2l(v));}
	

public:
	uint id;
	Matr M_g2l;	//transformation matrix: Global to Local
	Matr M_l2g;	//transformation matrix: Global to Local
};
//-----------------------------------------------------
class MrBox:public MrShape
{
public:
	MrBox():MrShape(){};
	MrBox(const MrObject &o):MrShape(o){std::cout<<"Created "<<ShapeType()<<std::endl;};
	MrBox(Vec3  scl,Vec3  ang,Vec3  pos):MrShape(scl,ang,pos){std::cout<<"Created "<<ShapeType()<<std::endl;};
	//MrBox(Vec3& pos,Vec3& ang,Vec3& scl):MrShape(scl,ang,pos){};
	//some virtual functions: 
	virtual const char *ShapeType(){return "Box";}
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
	MrSphere():MrShape(){};
	MrSphere(const MrObject &o):MrShape(o){};
	MrSphere(Vec3  scl,Vec3  ang,Vec3  pos):MrShape(scl,ang,pos){};
	//MrSphere(Vec3& pos,Vec3& ang,Vec3& scl):MrShape(scl,ang,pos){};
	//some virtual functions: 
	virtual const char* ShapeType(){return "Sphere";}
	//for output
	virtual void Print();
	//for calculations in LOCAL frame
	virtual double Volume_LOC();
	virtual bool IsInside_LOC(Vect v);
};

#endif