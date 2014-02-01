#ifndef __BASIC_TYPES
#define __BASIC_TYPES

#include <iostream>
#include <glm/glm.hpp> 
//#include <glm/gtc/constants.hpp> 
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 Vec3;
typedef glm::vec4 Vect;
typedef glm::mat4 Matr;

inline void rotate_M(Matr &M,float ang,Vec3 axis){
	M=glm::rotate(M,ang,axis);
}
inline void scale_M (Matr &M, Vec3 scl){
	M=glm::scale (M,scl);
}
inline void translate_M(Matr &M,Vec3 shift){
	M=glm::translate(M,shift);
}
inline void transpose_M(Matr &M){
	M=glm::transpose(M);
}

inline float DeterM3(Matr M){
	return 	 M[0][0]*M[1][1]*M[2][2]
			+M[0][1]*M[1][2]*M[2][0]
			+M[0][2]*M[1][0]*M[2][1]
			-M[0][0]*M[1][2]*M[2][1]
			-M[0][1]*M[1][0]*M[2][2]
			-M[0][2]*M[1][1]*M[2][0];
}

std::ostream& operator<<(std::ostream& out, const Vec3& v); // output
std::ostream& operator<<(std::ostream& out, const Vect& v); // output
std::ostream& operator<<(std::ostream& out, const Matr& m); // output


#endif