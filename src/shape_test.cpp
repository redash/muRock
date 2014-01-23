#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include "MrShape.h"

using namespace std;

#include <glm/gtc/random.hpp> 

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
	//тело:
	test_obj[0]=new MrSphere();
	test_obj[0]->Scl()=Vec3(20,10,10);
	test_obj[0]->Pos()=Vec3(0,0,0);
	test_obj[0]->Ang()=Vec3(0,0,0);
	//голова
	test_obj[1]=new MrSphere();
	test_obj[1]->Scl()=Vec3(8,8,8);
	test_obj[1]->Pos()=Vec3(15,0,8);
	test_obj[1]->Ang()=Vec3(0,0,0);
	//ушко левое
	test_obj[2]=new MrBox();
	test_obj[2]->Scl()=Vec3(2,4,12);
	test_obj[2]->Pos()=Vec3(15,-6,16);
	test_obj[2]->Ang()=Vec3(15,5,0);
	//ушко правое
	test_obj[3]=new MrBox();
	test_obj[3]->Scl()=Vec3(2,4,12);
	test_obj[3]->Pos()=Vec3(15,6,16);
	test_obj[3]->Ang()=Vec3(-15,5,0);
	//хвостик
	test_obj[4]=new MrSphere();
	test_obj[4]->Scl()=Vec3(5,5,5);
	test_obj[4]->Pos()=Vec3(-17,0,7);
	test_obj[4]->Ang()=Vec3(0,0,0);
	//лапка левая передняя
	test_obj[5]=new MrBox();
	test_obj[5]->Scl()=Vec3(3,3,10);
	test_obj[5]->Pos()=Vec3(12,-6,-8);
	test_obj[5]->Ang()=Vec3(0,0,0);
	//лапка правая передняя
	test_obj[6]=new MrBox();
	test_obj[6]->Scl()=Vec3(3,3,10);
	test_obj[6]->Pos()=Vec3(12,6,-8);
	test_obj[6]->Ang()=Vec3(0,0,0);
	//лапка левая задняя
	test_obj[7]=new MrBox();
	test_obj[7]->Scl()=Vec3(3,3,10);
	test_obj[7]->Pos()=Vec3(-12,-6,-8);
	test_obj[7]->Ang()=Vec3(0,0,0);
	//лапка правая задняя
	test_obj[8]=new MrBox();
	test_obj[8]->Scl()=Vec3(3,3,10);
	test_obj[8]->Pos()=Vec3(-12,6,-8);
	test_obj[8]->Ang()=Vec3(0,0,0);
	for(uint n=0;n<test_obj.size();++n)
		test_obj[n]->UpdMatrix();
	

	//make a small MC to determine a volume
	const int Npoints=100000;
	const double lim=30;
	Vect p(1);
	int nhits=0;
	//open file for points
	ofstream F;
	F.open("shp.txt", std::ofstream::out);
	//----------------------------------
	for(int n=0;n<Npoints;++n){
		//generate point
		p[0]=glm::linearRand(-lim,lim);
		p[1]=glm::linearRand(-lim,lim);
		p[2]=glm::linearRand(-lim,lim);
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
	MrShape obj;
	obj.Scl()=Vec3(glm::linearRand(0.01,100.0),glm::linearRand(0.01,100.0),glm::linearRand(0.01,100.0));
	obj.Ang()=Vec3(glm::linearRand(0.,360.),glm::linearRand(0.,180.),glm::linearRand(0.,360.));
	obj.Pos()=Vec3(glm::linearRand(-100.,100.),glm::linearRand(-100.,100.),glm::linearRand(-100.,100.));
	obj.UpdMatrix();
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

