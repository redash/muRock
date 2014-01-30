#include "basic_types.h"

std::ostream& operator<<(std::ostream& out, const Vec3& v) // output
{
    out<<"["<<v[0]<<",\t"<<v[1]<<",\t"<<v[2]<<"]";
    return out;
}
std::ostream& operator<<(std::ostream& out, const Vect& v) // output
{
    out<<"["<<v[0]<<",\t"<<v[1]<<",\t"<<v[2]<<",\t"<<v[3]<<"]";
    return out;
}

 std::ostream& operator<<(std::ostream& out, const Matr& m) // output
{
    out<<"|"<<m[0]<<"|\n";
    out<<"|"<<m[1]<<"|\n";
    out<<"|"<<m[2]<<"|\n";
    out<<"|"<<m[3]<<"|\n";
    return out;
}