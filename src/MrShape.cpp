#include <iostream>
#include "MrShape.h"


using namespace std;
ostream& operator<<(ostream& out, const Vec3& v) // output
{
    out<<"["<<v[0]<<",\t"<<v[1]<<",\t"<<v[2]<<"]";
    return out;
}
ostream& operator<<(ostream& out, const Vect& v) // output
{
    out<<"["<<v[0]<<",\t"<<v[1]<<",\t"<<v[2]<<",\t"<<v[3]<<"]";
    return out;
}

 ostream& operator<<(ostream& out, const Matr& m) // output
{
    out<<"|"<<m[0]<<"|"<<endl;
    out<<"|"<<m[1]<<"|"<<endl;
    out<<"|"<<m[2]<<"|"<<endl;
    out<<"|"<<m[3]<<"|"<<endl;
    return out;
}


static uint MaxId=0;
//------------------------------------------------------
MrObject::MrObject(uint Id):id(Id),pos(1),ang(0),scl(1){
}
//------------------------------------------------------
void MrObject::Print(){
		cout<<"**** Object #"<<id<<endl;
		cout<<"pos="<<pos<<endl;
		cout<<"ang="<<ang<<endl;
		cout<<"scl="<<scl<<endl;
}

//====================================================
//            Mr SHAPE class:
//====================================================

//---------------------------------------
MrShape::MrShape():M_g2l(1),M_l2g(1){
	id=MaxId++;
	printf("Created shape#%d\n",id);
}
//---------------------------------------
MrShape::MrShape(const Vec3& scl,const Vec3& ang,const Vec3& pos)
:M_g2l(1),M_l2g(1)
{
	id=MaxId++;
	cout<<"Create shape#"<<id<<endl;
	cout<<"pos="<<pos<<endl;
	cout<<"ang="<<ang<<endl;
	cout<<"scl="<<scl<<endl;
	UpdateM(pos,ang,scl);
}
//---------------------------------------
MrShape::~MrShape(){
	cout<<"Delete shape#"<<id<<endl;
}
//---------------------------------------
void MrShape::InitM(){
	M_g2l=Matr(1);
	M_l2g=M_g2l;
}
//---------------------------------------
void MrShape::TranslateM(const Vec3 &shift){
	//apply translation	
	Matr M_pos=glm::translate(Matr(1),shift);
	M_l2g=M_pos*M_l2g;
	M_pos[3][0]=-M_pos[3][0];
	M_pos[3][1]=-M_pos[3][1];
	M_pos[3][2]=-M_pos[3][2];
	M_g2l=M_g2l*M_pos;
} 
//---------------------------------------
void MrShape::ScaleM(const Vec3 &scale){//apply scaling
	Matr M_scl=glm::scale (Matr(1),scale);
	M_l2g=M_scl*M_l2g;
	M_scl[0][0]=1./M_scl[0][0];
	M_scl[1][1]=1./M_scl[1][1];
	M_scl[2][2]=1./M_scl[2][2];
	M_l2g=M_l2g*M_scl;
}
//---------------------------------------
void MrShape::RotateM(float angle,const Vec3 &axis){
	Matr M_rot=glm::rotate(Matr(1),angle,axis);
	M_l2g=M_rot*M_l2g;
	M_rot=glm::transpose(M_rot);
	M_g2l=M_g2l*M_rot;
}
//---------------------------------------
void MrShape::UpdateM(const Vec3 &pos,const Vec3 &ang,const Vec3 &scl){
	Matr M_scl(1),M_rot(1),M_pos(1);

	M_scl=glm::scale (M_scl,scl);
	M_rot=glm::rotate(M_rot,ang[0],Vec3(1,0,0)); //psi
	M_rot=glm::rotate(M_rot,ang[1],Vec3(0,1,0)); //theta
	M_rot=glm::rotate(M_rot,ang[2],Vec3(0,0,1)); //phi
	M_pos=glm::translate(M_pos,pos);
	M_l2g=M_g2l*M_pos*M_rot*M_scl;
	// update inverse matrix
	M_rot=glm::transpose(M_rot);
	for(int n=0;n<3;++n){
		M_scl[n][n]=1./M_scl[n][n];
		M_pos[3][n]=-M_pos[3][n];
	}
	M_g2l=M_scl*M_rot*M_pos*M_g2l;
}
//---------------------------------------
void MrShape::Print(){
	cout<<"Mr."<<this->ShapeType()<<" says:"<<"\"How do you do?\""<<endl;
	cout<<M_g2l<<endl;
}
//---------------------------------------

//====================================================
//            Mr BOX class:
//====================================================
void MrBox::Print(){
	MrShape::Print();
}
//---------------------------------------
double MrBox::Volume_LOC(){return 8.0;}
//---------------------------------------
bool   MrBox::IsInside_LOC(Vect v){return (fabs(v[0])<=1.0 && fabs(v[1])<=1.0 && fabs(v[2])<=1.0);}
//---------------------------------------
//====================================================
//            MrsPhere class:
//====================================================
void MrSphere::Print(){
	MrShape::Print();
}
//---------------------------------------
double MrSphere::Volume_LOC(){return (4./3)*3.1415;}
//---------------------------------------
bool   MrSphere::IsInside_LOC(Vect v){	return (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]<=1.0);	}

//====================================================
//            MrCylinder class:
//====================================================
void MrCylinder::Print(){
	MrShape::Print();
}
//---------------------------------------
double MrCylinder::Volume_LOC(){return 2.0*3.1415;}
//---------------------------------------
bool   MrCylinder::IsInside_LOC(Vect v){	return (v[0]*v[0]+v[1]*v[1]<=1.0)&&(fabs(v[2])<=1.0);	}