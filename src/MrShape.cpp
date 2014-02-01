#include <iostream>
#include <algorithm>
#include "MrShape.h"
#include "MrRay.h"
using namespace std;

static uint MaxId=1;
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
	UpdateM(scl,ang,pos);
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
	Matr M_pos(1);
	translate_M(M_pos,shift);
	M_l2g=M_pos*M_l2g;
	M_pos[3][0]=-M_pos[3][0];
	M_pos[3][1]=-M_pos[3][1];
	M_pos[3][2]=-M_pos[3][2];
	M_g2l=M_g2l*M_pos;
} 
//---------------------------------------
void MrShape::ScaleM(const Vec3 &scale){//apply scaling
	Matr M_scl(1);
	scale_M(M_scl,scale);
	M_l2g=M_scl*M_l2g;
	M_scl[0][0]=1./M_scl[0][0];
	M_scl[1][1]=1./M_scl[1][1];
	M_scl[2][2]=1./M_scl[2][2];
	M_g2l=M_g2l*M_scl;
}
//---------------------------------------
void MrShape::RotateM(float angle,const Vec3 &axis){
	Matr M_rot(1);
	rotate_M(M_rot,angle,axis);
	M_l2g=M_rot*M_l2g;
	transpose_M(M_rot);
	M_g2l=M_g2l*M_rot;
}
//---------------------------------------
void MrShape::UpdateM(const Vec3 &scl,const Vec3 &ang,const Vec3 &pos){
	Matr M_scl(1),M_rot(1),M_pos(1);

	scale_M (M_scl,scl);
	rotate_M(M_rot,ang[0],Vec3(1,0,0)); //psi
	rotate_M(M_rot,ang[1],Vec3(0,1,0)); //theta
	rotate_M(M_rot,ang[2],Vec3(0,0,1)); //phi
	translate_M(M_pos,pos);
	M_l2g=M_l2g*M_pos*M_rot*M_scl;
	// update inverse matrix
	transpose_M(M_rot);
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
	cout<<M_l2g<<endl;
}
//---------------------------------------
int MrShape::Intersect(MrRay & ray){
	//store GLOBAL vectors
	Vect r0_glob=ray.R0();
	Vect  v_glob=ray.V ();
	// transform Ray to LOCAL frame
	ray.R0()=g2l(ray.R0());
	ray.V() =g2l(ray.V() );
	// intersect locally
	int result=Intersect_LOC(ray);
	//restore GLOBAL vectors in Ray
	ray.R0()=r0_glob;
	ray.V ()= v_glob;
	return result;
};
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
bool   MrBox::IsInside_LOC(Vect v){
	return (fabs(v[0])<=1.0 && fabs(v[1])<=1.0 && fabs(v[2])<=1.0);
}
//---------------------------------------
int MrBox::Intersect_LOC(MrRay & ray){
	float lim[2]={-1.0,1.0}; //box limits (x,y,z)
	float t;
	Vect r0=ray.R0();
	Vect v =ray.V();
	Vect r;
	int np=0;
	for(int ax=0; ax<3;++ax){
		if(v[ax]==0)continue;
		for(int nlim=0; nlim<2;++nlim){
			//calculate time of plane intersection:
			t=(lim[nlim]-r0[ax])/v[ax];
			//cout<<"lim="<<lim[nlim]<<", v[ax]="<<v[ax]<<", t="<<t<<endl;
			//propagate ray to this t:
			r=ray.R(t);
			r[ax]=0;
			if(IsInside_LOC(r)){
				ray.AddCrossPoint(CrossPoint(id,v[ax]*lim[nlim],t));
				//cout<<"Isect!: "<<id<<" flag="<<v[ax]*lim[nlim]<<", t="<<t<<"\t r="<<r<<endl;
				++np;
			}
		}

	}
	return np;
}
//---------------------------------------
//====================================================
//            MrsPhere class:
//====================================================
void MrSphere::Print(){
	MrShape::Print();
}
//---------------------------------------
int MrSphere::Intersect_LOC(MrRay & ray){
// intersection checks: fill crossing points in ray, 
// return number of crossing points
	float v2=ray.V_2();
	float r2=ray.R0_2();
	float rv=ray.R0_V();
	float D=rv*rv-v2*(r2-1);
	//cout<<"v2="<<v2<<", r2="<<r2<<", rv="<<rv<<",\nD="<<D<<endl;
	if(D<=0)return 0; ///no intersection or 1 point
	float sqD=sqrt(D);
	ray.AddCrossPoint(CrossPoint(id,-1,(-rv-sqD)/v2));
	ray.AddCrossPoint(CrossPoint(id,+1,(-rv+sqD)/v2));
	return 2;
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
bool   MrCylinder::IsInside_LOC(Vect v){
	// check if point is inside volume
	return (v[0]*v[0]+v[1]*v[1]<=1.0)&&(fabs(v[2])<=1.0);
}
//---------------------------------------
int MrCylinder::Intersect_LOC(MrRay & ray){
// intersection checks: fill crossing points in ray, 
// return number of crossing points
	Vect v=ray.V();
	Vect r=ray.R0();
	
	const float zlim[2]={-1,1};
	
	float t[2]={0,0};
	float t_side[2]={0,0};
	float t_caps[2]={0,0};

	///check for crossings with caps:
	for(unsigned i = 0; i < 2; ++i)
		t_caps[i]=(zlim[i]-r[2])/v[2];

	if(t_caps[1]<t_caps[0])swap(t_caps[0],t_caps[1]); //T-order

	//check if intersection with caps is inside cylinder:
	/*float rT[2];
	for(unsigned i = 0; i < 2; ++i) {
		rT[0]=ray.X(t[n_cap]);
		rT[1]=ray.Y(t[n_cap]);
		hit_caps[n_cap]=(rT[0]*rT[0]+rT[1]*rT[1]<1.0); //true if cap was hit
	}*/
	
	
	float v2=v[0]*v[0]+v[1]*v[1];
	float r2=r[0]*r[0]+r[1]*r[1];
	if(v2>0){
		// if(v2==0)return 0;
		//calculate intersections with side surface
		float rv=v[0]*r[0]+v[1]*r[1];
		float D=rv*rv-v2*(r2-1);
		//cout<<"v2="<<v2<<", r2="<<r2<<", rv="<<rv<<",\nD="<<D<<endl;
		if(D<=0)return 0;
		float sqD=sqrt(D);
		for (unsigned i = 0; i < 2; ++i)
			t_side[i]=(-rv+zlim[i]*sqD)/v2;

		t[0]=max(t_caps[0],t_side[0]);
		t[1]=min(t_caps[1],t_side[1]);
	}
	else{
		if(r2>=1)return 0;
		else {t[0]=t_caps[0]; t[1]=t_caps[1]; }
	}
	if(t[0]>=t[1])return 0; //no intersection
	ray.AddCrossPoint(CrossPoint(id,-1,t[0]));
	ray.AddCrossPoint(CrossPoint(id,+1,t[1]));
	return 2;
}
//====================================================
//            MrPyramid class:
//====================================================
void MrPyramid::Print(){
	MrShape::Print();
}
//---------------------------------------
double MrPyramid::Volume_LOC(){return 8.0/3;}
//---------------------------------------
bool   MrPyramid::IsInside_LOC(Vect v){
	if(fabs(v[2])>1.0)return false;
	float a1=0.5*(1.0-v[2]);
	return (fabs(v[0])<=a1 && fabs(v[1])<=a1);	
}

