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
MrShape::~MrShape(){
	printf("Delete shape#%d\n",id);
}
//---------------------------------------
void MrShape::UpdMatrix(){
	M_l2g=Matr(1); M_g2l=Matr(1);

	Matr M_scl(1),M_rot(1),M_pos(1);

	M_scl=glm::scale (M_scl,scl);
	M_rot=glm::rotate(M_rot,ang[0],Vec3(1,0,0));
	M_rot=glm::rotate(M_rot,ang[1],Vec3(0,1,0));
	M_rot=glm::rotate(M_rot,ang[2],Vec3(0,0,1));
	M_pos=glm::translate(M_pos,pos);
	M_l2g=M_pos*M_rot*M_scl;

	// update inverse matrix
	M_rot=glm::transpose(M_rot);
	for(int n=0;n<3;++n){
		M_scl[n][n]=1./M_scl[n][n];
		M_pos[3][n]=-M_pos[3][n];
	}
	M_g2l=M_scl*M_rot*M_pos;
}
//---------------------------------------
void MrShape::Print(){
	cout<<M_g2l<<endl;
	MrObject::Print();
}
//---------------------------------------
/*bool MrShape::Chk_crude_sph_LOC(Vect v){
	return (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]<1.0);
};
*/
//====================================================
//            Mr BOX class:
//====================================================
void MrBox::Print(){
	cout<<"Mr.Box says: \"How do you do?\" "<<endl;
	MrShape::Print();
}
//---------------------------------------
double MrBox::Volume_LOC(){return 8.0;}
//---------------------------------------
bool   MrBox::IsInside_LOC(Vect v){return (fabs(v[0])<=1.0 && fabs(v[1])<=1.0 && fabs(v[2])<=1.0);}
//====================================================
//            MrsPhere class:
//====================================================
void MrSphere::Print(){
	cout<<"Mrs.Phere says: \"How do you do?\" "<<endl;
	MrShape::Print();
}
//---------------------------------------
double MrSphere::Volume_LOC(){return (4./3)*3.1415;}
//---------------------------------------
bool   MrSphere::IsInside_LOC(Vect v){	return (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]<1.0);	}