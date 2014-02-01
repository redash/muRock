#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include "basic_types.h"
#include "MrShape.h"
#include "MrRay.h"
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
	bool TestMC(float lim=30,int Npt=10000);
	bool TestLattice(float lim=30,int Npt=50);
	bool TestUnit();
	bool TestRays(float lim=50,int Npt=200);
	void operator +=(MrShape* shp){test_obj.push_back(shp);}
	void Clear();
private:
	string Name;
	std::vector<MrShape*> test_obj;
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
	Clear();
}
//--------------------------------------------------------
void shape_test::Clear(){
	//for(uint n=0;n<test_obj.size();++n)
	while(!test_obj.empty()){
		delete test_obj.back();
		test_obj.pop_back();
	}
}
//--------------------------------------------------------
void shape_test::Print(){
	cout<<"Hi! I am test "<<Name<<endl;
	for(auto&& o : test_obj) {
		o->Print();
	}
}
//--------------------------------------------------------
bool shape_test::TestMC(float lim,int Npt){
	cout<<"================== start MC TEST ================== "<<endl;
	//make a small MC to determine a volume
	const int Npoints=Npt;
	Vect p;
	int nhits=0;
	//open file for points
	ofstream F;
	F.open("shp.txt", std::ofstream::out);
	F<<"LIM "<<-lim<<" "<<-lim<<" "<<-lim<<endl;
	F<<"LIM "<< lim<<" "<<-lim<<" "<<-lim<<endl;
	F<<"LIM "<< lim<<" "<< lim<<" "<<-lim<<endl;
	F<<"LIM "<<-lim<<" "<< lim<<" "<<-lim<<endl;
	F<<"LIM "<<-lim<<" "<<-lim<<" "<< lim<<endl;
	F<<"LIM "<< lim<<" "<<-lim<<" "<< lim<<endl;
	F<<"LIM "<< lim<<" "<< lim<<" "<< lim<<endl;
	F<<"LIM "<<-lim<<" "<< lim<<" "<< lim<<endl;
	//----------------------------------
	for(int n=0;n<Npoints;++n){
		//generate point
		p[0]=frand(-lim,lim);
		p[1]=frand(-lim,lim);
		p[2]=frand(-lim,lim);
		p[3]=1;
		for(auto&& o : test_obj)
			if(o->IsInside(p)){
				// !point is inside!!
				F<<"HIT "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
				++nhits;
			}
			else F<<"... "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
	}

	F.close();
	
	auto V0=8.0*lim*lim*lim; //"world" volume
	auto V_mc=(V0*nhits)/Npoints;	//MC estimated volume
	
	double V_calc=0;
	for(auto&& o : test_obj) V_calc+=o->Volume();
	
	auto hits_calc=V_calc*Npoints/V0;
	//auto V_calc=(4/3.)*3.1415*test_obj.Scl()[0]*test_obj.Scl()[1]*test_obj.Scl()[2];

	cout<<nhits<<"/"<<Npoints<<" hits! in cubic volume"<<2*lim<<"^3 ("<<hits_calc<<" hits expected)"<<endl;
	cout<<"V_mc  ="<<V_mc<<endl;
	cout<<"V_calc="<<V_calc<<endl;
	
	auto Diff=V_mc-V_calc;
	auto rDiff=100*Diff/V_calc;
	auto rStat=100./sqrt(hits_calc);
	
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
bool shape_test::TestLattice(float lim,int Npt){
	cout<<"================== start LATTICE TEST ================== "<<endl;

	Vect p(1);
	int Npoints=Npt*Npt*Npt;
	int nhits=0;
	//open file for points
	ofstream F;
	F.open("shp_lat.txt", std::ofstream::out);
	F<<"LIM "<<-lim<<" "<<-lim<<" "<<-lim<<endl;
	F<<"LIM "<< lim<<" "<<-lim<<" "<<-lim<<endl;
	F<<"LIM "<< lim<<" "<< lim<<" "<<-lim<<endl;
	F<<"LIM "<<-lim<<" "<< lim<<" "<<-lim<<endl;
	F<<"LIM "<<-lim<<" "<<-lim<<" "<< lim<<endl;
	F<<"LIM "<< lim<<" "<<-lim<<" "<< lim<<endl;
	F<<"LIM "<< lim<<" "<< lim<<" "<< lim<<endl;
	F<<"LIM "<<-lim<<" "<< lim<<" "<< lim<<endl;
	//----------------------------------
	float dx=2*lim/Npt; //step size
	for(p[0]=-lim;p[0]<=lim;p[0]+=dx)
		for(p[1]=-lim;p[1]<=lim;p[1]+=dx)
			for(p[2]=-lim;p[2]<=lim;p[2]+=dx){

				for(auto&& o : test_obj)
					if(o->IsInside(p)){
						// !point is inside!!
						F<<"HIT "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
						++nhits;
						break;
					}

		        F<<"... "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
	}
	F.close();
	
	auto V0=8.0*lim*lim*lim; //"world" volume
	auto V_mc=(V0*nhits)/Npoints;	//MC estimated volume
	
	double V_calc=0;
	for(auto&& o : test_obj)V_calc+=o->Volume();
	auto hits_calc=V_calc*Npoints/V0;

	cout<<nhits<<"/"<<Npoints<<" hits! in cubic volume"<<2*lim<<"^3 ("<<hits_calc<<" hits expected)"<<endl;
	cout<<"V_latt  ="<<V_mc<<endl;
	cout<<"V_calc="<<V_calc<<endl;
	
	auto Diff=V_mc-V_calc;
	auto rDiff=100*Diff/V_calc;
	auto rStat=100./sqrt(hits_calc);
	
	cout<<"Diff="<<Diff<<" ("<<rDiff<<"\%, stat ="<<rStat<<")"<<endl;
	// check accuracy of MC expectation
	if(fabs(rDiff/rStat)>=2){ //2 sigma confidence
		cout<<"================== LATTICE TEST: FAILED "<<endl;
		cerr<<"Error: LATTICE volume test FAILED"<<endl;
		return false;
	}
	cout<<"================== LATTICE TEST: passed "<<endl;
	return true;
}
//--------------------------------------------------------
bool shape_test::TestRays(float lim,int Npt){
	cout<<"================== start RAY TEST ================== "<<endl;

	Vect p(0,0,lim,1);
	Vect v(0.1,0.4,-1,0);
	int nhits=0;
	//open file for points
	ofstream F;
	F.open("shp_ray.txt", std::ofstream::out);
	// F.precision(5);
	// F.setf(std::ios::fixed,std:: ios::floatfield );
	//----------------------------------
	float dx=2*lim/Npt; //step size
	//write limits
	F<<"LIM "<<-lim<<" "<<-lim<<" "<<-lim<<endl;
	F<<"LIM "<< lim<<" "<<-lim<<" "<<-lim<<endl;
	F<<"LIM "<< lim<<" "<< lim<<" "<<-lim<<endl;
	F<<"LIM "<<-lim<<" "<< lim<<" "<<-lim<<endl;
	F<<"LIM "<<-lim<<" "<<-lim<<" "<< lim<<endl;
	F<<"LIM "<< lim<<" "<<-lim<<" "<< lim<<endl;
	F<<"LIM "<< lim<<" "<< lim<<" "<< lim<<endl;
	F<<"LIM "<<-lim<<" "<< lim<<" "<< lim<<endl;
	for(p[0]=-lim;p[0]<=lim;p[0]+=dx)
		for(p[1]=-lim;p[1]<=lim;p[1]+=dx){
				MrRay ray(p,v);
				//cout<<"Ray:\t"<<ray.R0()<<" "<<ray.V()<<endl;
				for(auto&& o : test_obj){
					nhits=o->Intersect(ray);
					if(nhits>0){
						// !point is inside!!
						// cout<<"Hits=%d\n";
						for(auto&& h : ray.cpoints){
							Vect r=ray.R(h.t);
							//cout<<h.In()<<" "<<h.t<<"\t"<<r<<endl;
							if(h.Out())F<<"... "<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
							else F<<"HIT "<<r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
						}
					}
				}
	}
	F.close();
	cout<<"================== RAY TEST: passed "<<endl;
	return true;
}
//--------------------------------------------------------

bool shape_test::TestUnit(){
	const double Eps=1e-6;
	double res=0,MaxRes=0,SumRes=0;

	cout<<"================== start UNITARITY TEST ============"<<endl;
	for(uint n=0;n<test_obj.size();++n){
		MrShape *obj=test_obj[n];
		Matr M1=obj->M_g2l*obj->M_l2g;
		Matr I(1);
		M1=M1-I;
	
		//check unitarity
		for(int nx=0;nx<4;++nx)
			for(int ny=0;ny<4;++ny){
				res=fabs(M1[nx][ny]);
				SumRes+=res;
				if(res>MaxRes)MaxRes=res;
			}
	}
	printf("Maximum deviation = %g\n",MaxRes);
	printf("Summary deviation = %g\n",SumRes/test_obj.size());
	if(MaxRes>Eps){
				cout<<"================== UNITARITY TEST: FAILED "<<endl;
				cerr<<"Error: UNITARITY test FAILED"<<endl;
				return false;
	}
	cout<<"================== UNITARITY TEST: passed "<<endl;
	return true;
}
//--------------------------------------------------------
bool shape_test::DoTest(){
	
	 // return TestUnit() && TestMC() && TestLattice() && TestRays();
	return TestRays();
}
//--------------------------------------------------------
int main(int argc,char** argv){
	srand(time(NULL));	
	cout.width(20);
	cout<<std::setprecision(3)<<std::fixed;
	shape_test Test("NewTest");
	//рисуем зайчика
	Test+=new MrCylinder(Vec3{ 10, 10, 20},Vec3{  0,90, 0},Vec3{  0, 0, 0} ); //тело
	Test+=new MrCylinder(Vec3{  8,  8,  8},Vec3{  0,90, 0},Vec3{ 15, 0, 8} ); //голова
	Test+=new MrBox		(Vec3{  2,  4, 12},Vec3{ 15, 5, 0},Vec3{ 15,-6,16} ); //ушко левое
	Test+=new MrBox		(Vec3{  2,  4, 12},Vec3{-15, 5, 0},Vec3{ 15, 6,16} ); //ушко правое 
	Test+=new MrSphere	(Vec3{  5,  5, 5 },Vec3{  0, 0, 0},Vec3{-17, 0, 7} ); //хвостик
	Test+=new MrBox		(Vec3{  3,  3, 10},Vec3{  0, 0, 0},Vec3{ 12,-6,-8} ); //лапка левая передняя
	Test+=new MrBox		(Vec3{  3,  3, 10},Vec3{  0, 0, 0},Vec3{ 12, 6,-8} ); //лапка правая передняя ;
	Test+=new MrBox		(Vec3{  3,  3, 10},Vec3{  0, 0, 0},Vec3{-12,-6,-8} ); //лапка левая задняя ;
	Test+=new MrBox		(Vec3{  3,  3, 10},Vec3{  0, 0, 0},Vec3{-12, 6,-8} ); //лапка правая задняя ;

// Test+=new MrCylinder(Vec3{  20,  20,  20},Vec3{  0,  30, 30}, Vec3{  0, 0, 0} ); //X
	//Test+=new MrSphere(Vec3{  40,  5,  5},Vec3{  0,  0, 0}, Vec3{  0, 20, 0} ); //Y

//spherical rabbit
	/*Test+=new MrSphere(Vec3(15,15,15), Vec3(  0, 0, 0) ,Vec3(0,0,0)   );  //cout<<"тело "<<endl;
    Test+=new MrSphere(Vec3(10,10,10),    Vec3( 0,0,0) ,Vec3(0,0,20)  );  //cout<<"голова  "<<endl;
    Test+=new MrSphere(Vec3(3,3,3),    Vec3( 0,0,0) ,Vec3(0,-9,20)    );  //cout<<"носик  "<<endl;
    Test+=new MrSphere(Vec3(5,2,15),   Vec3(-10,-20, 0) ,Vec3(-10,0,30) );//cout<<"ушко левое  "<<endl;
    Test+=new MrSphere(Vec3(5,2,15),   Vec3(-10, 20, 0) ,Vec3( 10,0,30) );//cout<<"ушко правое  "<<endl;
    Test+=new MrSphere(Vec3(5,5,5 ),   Vec3(  0, 0, 0) ,Vec3(0,17,-2) );//cout<<"хвостик  "<<endl;
    Test+=new MrSphere(Vec3(4,4,10),   Vec3(55,-15,-15) ,Vec3(-10,-10,10) );//cout<<"лапка левая передняя  "<<endl;
    Test+=new MrSphere(Vec3(4,4,10),   Vec3(55,15,15) ,Vec3(10,-10,10) );//cout<<"лапка правая передняя  "<<endl;
    Test+=new MrSphere(Vec3(5,5,7),   Vec3(-20, 0, 0) ,Vec3(-12,-12,-9) );//cout<<"лапка левая задняя  "<<endl;
    Test+=new MrSphere(Vec3(5,5,7),   Vec3(-20, 0, 0) ,Vec3(12,-12,-9) );//cout<<"лапка правая задняя  "<<endl;
    */
    Test.Print();
	bool res=Test.DoTest();

	return res?0:1;
}

