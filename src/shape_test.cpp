#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include "MrShape.h"

using namespace std;

//#include <glm/gtc/random.hpp> 
float  frand(float a, float b){return (rand()*1.0f/RAND_MAX)*(b-a)+a; }
class shape_test
{
public:
	shape_test(string new_name);
	~shape_test();
	void Print();
	bool DoTest();
	bool TestMC();
	bool TestUnit();
private:
	string Name;
};

// class implementation
//--------------------------------------------------------
shape_test::shape_test(string new_name):Name(new_name){
	//c-tor
	cout<<"**** Begin shape_test ["<<Name<<"] created!\n";
}
//--------------------------------------------------------
shape_test::~shape_test(){
	//d-tor
	cout<<"**** Stop shape_test ["<<Name<<"]...\n";
}
//--------------------------------------------------------
void shape_test::Print(){
	cout<<"Hi! I am "<<Name<<endl;
}
//--------------------------------------------------------
bool shape_test::TestMC(){
	cout<<"================== start MC TEST ================== "<<endl;
	
	std::vector<MrShape*> test_obj(9);
	
	//MrBox test_obj=MrBox();
	//рисуем зайчика
	test_obj[0]=new MrSphere(Vec3(20,10,10), Vec3(  0, 0, 0) ,Vec3(0,0,0)	);	//cout<<"тело "<<endl;
	test_obj[1]=new MrSphere(Vec3(8,8,8),    Vec3(  0, 0, 0) ,Vec3(15,0,8) 	); 	//cout<<"голова  "<<endl;
	test_obj[2]=new MrBox	(Vec3(2,4,12),   Vec3( 15, 5, 0) ,Vec3( 15,-6,16) );//cout<<"ушко левое  "<<endl;
	test_obj[3]=new MrBox	(Vec3(2,4,12),   Vec3(-15, 5, 0) ,Vec3( 15, 6,16) );//cout<<"ушко правое  "<<endl;
	test_obj[4]=new MrSphere(Vec3(5,5,5 ),   Vec3(  0, 0, 0) ,Vec3(-17, 0, 7) );//cout<<"хвостик  "<<endl;
	test_obj[5]=new MrBox	(Vec3(3,3,10),   Vec3(  0, 0, 0) ,Vec3( 12,-6,-8) );//cout<<"лапка левая передняя  "<<endl;
	test_obj[6]=new MrBox	(Vec3(3,3,10),   Vec3(  0, 0, 0) ,Vec3( 12, 6,-8) );//cout<<"лапка правая передняя  "<<endl;
	test_obj[7]=new MrBox	(Vec3(3,3,10),   Vec3(  0, 0, 0) ,Vec3(-12,-6,-8) );//cout<<"лапка левая задняя  "<<endl;
	test_obj[8]=new MrBox	(Vec3(3,3,10),   Vec3(  0, 0, 0) ,Vec3(-12, 6,-8) );//cout<<"лапка правая задняя  "<<endl;

	for(uint n=0;n<test_obj.size();++n)	test_obj[n]->Print();
	//for(uint n=0;n<test_obj.size();++n)	test_obj[n]->UpdMatrix();
	

	//make a small MC to determine a volume
	const int Npoints=100000;
	const float lim=30;
	Vect p(1);
	int nhits=0;
	//open file for points
	ofstream F;
	F.open("shp.txt", std::ofstream::out);
	//----------------------------------
	for(int n=0;n<Npoints;++n){
		//generate point
		p[0]=frand(-lim,lim);
		p[1]=frand(-lim,lim);
		p[2]=frand(-lim,lim);
		p[3]=1;
		for(uint n=0;n<test_obj.size();++n)
			if(test_obj[n]->IsInside(p)){
				// !point is inside!!
				F<<"HIT "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
				++nhits;
			}
		else F<<"... "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
		}
	F.close();
	
	double V0=8.0*lim*lim*lim; //"world" volume
	double V_mc=(V0*nhits)/Npoints;	//MC estimated volume
	
	double V_calc=0;
	for(uint n=0;n<test_obj.size();++n) V_calc+=test_obj[n]->Volume();
	double hits_calc=V_calc*Npoints/V0;
	//double V_calc=(4/3.)*3.1415*test_obj.Scl()[0]*test_obj.Scl()[1]*test_obj.Scl()[2];

	cout<<nhits<<"/"<<Npoints<<" hits! in cubic volume"<<2*lim<<"^3 ("<<hits_calc<<" hits expected)"<<endl;
	cout<<"V_mc  ="<<V_mc<<endl;
	cout<<"V_calc="<<V_calc<<endl;
	
	double Diff=V_mc-V_calc;
	double rDiff=100*Diff/V_calc;
	double rStat=100./sqrt(hits_calc);
	
	cout<<"Diff="<<Diff<<" ("<<rDiff<<"\%, stat ="<<rStat<<")"<<endl;
	// check accuracy of MC expectation
	if(fabs(rDiff/rStat)>=2){ //2 sigma confidence :)
		cout<<"================== MC TEST: FAILED "<<endl;
		cerr<<"Error: MC volume test FAILED"<<endl;
		return false;
	}
	cout<<"================== MC TEST: passed "<<endl;
	return true;
}
//--------------------------------------------------------
bool shape_test::TestUnit(){
	cout<<"================== start UNITARITY TEST ============"<<endl;
	
	Vec3 scl=Vec3(frand(0.01,100.0),frand(0.01,100.0),	frand(0.01,100.0));
	Vec3 ang=Vec3(frand(0.,360.),	frand(0.,180.),		frand(0.,360.));
	Vec3 pos=Vec3(frand(-100.,100.),frand(-100.,100.),	frand(-100.,100.));
	MrShape obj(scl,ang,pos);
	obj.Print();
	Matr M1=obj.M_g2l*obj.M_l2g;
	Matr I(1);
	M1=M1-I;
	cout<<"Difference (M - I):"<<endl;
	cout<<M1<<endl;

	const double Eps=1e-6;
	//check unitarity
	for(int nx=0;nx<4;++nx)
		for(int ny=0;ny<4;++ny)
			if(M1[nx][ny]>Eps){
				cout<<"================== UNITARITY TEST: FAILED "<<endl;
				cerr<<"Error: UNITARITY test FAILED"<<endl;
				return false;
			}
	
	cout<<"================== UNITARITY TEST: passed "<<endl;
	return true;
}
//--------------------------------------------------------
bool shape_test::DoTest(){
	return TestUnit() && TestMC();
}
//--------------------------------------------------------
int main(int argc,char** argv){
	srand(time(NULL));
	cout.width(20);
	cout<<std::setprecision(3)<<std::fixed;
	shape_test Test("NewTest");
	printf("Hello\n");
	bool res=Test.DoTest();

	return res?0:1;
}

