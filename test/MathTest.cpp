#include "Test.h"
#include "sobj.h"

using namespace slib;

#ifdef MATH_TEST
using namespace slib::smath;

void test::CalcTest() {
	std::cout << "1.0f==1?" << (smath::isInteger(1.0f) ? "true" : "false") << std::endl; //true
	std::cout << "1.5==1 or 2?" << (smath::isInteger(1.5) ? "true" : "false") << std::endl; //false
	std::cout << "3//2=" << smath::quot(3, 2) << std::endl; //1
	std::cout << "-5//2=" << smath::quot(-5, 2) << std::endl; //-2
	std::cout << "4.5//2=" << smath::quot(4.5, 2) << std::endl; //2
	std::cout << "2^3=" << smath::power(2, 3) << std::endl; //8
	std::cout << "1.2f^2=" << smath::power(1.2f, 2) << std::endl; //1.44
	std::cout << "2.5^4=" << smath::power(2.5, 4) << std::endl;
	std::cout << "2.56^(1/2)=" << smath::rootN(2.56, 2) << std::endl; //1.6
	std::cout << "8^(1/3)=" << smath::rootN(8, 3) << std::endl; //2.0
	std::cout << "digit of 1234567=" << smath::digit(1234567) << std::endl;
	std::cout << "decim of 123.4567=" << smath::decim(123.4567) << std::endl;
	std::cout << "G.C.D.(16, 36)=" << smath::gcd(16, 36) << std::endl;
	std::cout << "L.C.M.(24, 56)=" << smath::lcm(24, 56) << std::endl;
	std::cout << "factorial(4, 2)=" << smath::factorial(4, 2) << std::endl;
	std::cout << "combination(6, 2)=" << smath::combination(6, 2) << std::endl;
	std::cout << "kronecker delta(1, 1)=" << smath::kdelta(1, 1) << std::endl;
	std::cout << "radian of 45=" << smath::deg2rad(45) << std::endl;
	std::cout << "degree of 1.57=" << smath::rad2deg(1.57) << std::endl;
	std::cout << "Random number test..." << std::endl;
	smath::SRandom rand;
	std::cout << "Int. Uni. in [1, 10]=" << rand.iruni(1, 10) << std::endl;
	std::cout << "Uni. in [0, 1]=" << rand.runi(0.0, 1.0) << std::endl;
	std::cout << "Norm. =" << rand.rnorm(0.0, 1.0) << std::endl;
	std::cout << "LogNorm. =" << rand.rlognorm(0.0, 1.0) << std::endl;
	std::cout << "ChiSq. =" << rand.rchisq(2) << std::endl;
	std::cout << "Pois. =" << rand.rpois(1.0) << std::endl;
	std::cout << "Exp. =" << rand.rexp(1.0) << std::endl;
	std::cout << "Gamma. =" << rand.rgamma(1.0, 1.0) << std::endl;
	std::cout << "Weib. =" << rand.rweib(1.0, 1.0) << std::endl;
	std::cout << "Bern. =" << rand.rbern(1.0) << std::endl;
	std::cout << "Binom. =" << rand.rbinom(2, 1.0) << std::endl;
	std::cout << "Geom. =" << rand.rgeom(0.5) << std::endl;
	std::cout << "Neg.Binom. =" << rand.rnbinom(2, 0.5) << std::endl;
	std::cout << "Rayleigh =" << rand.rrayleigh(1.0) << std::endl;
	std::cout << "Vonmises. =" << rand.rvonmises(0.0, 1.0) << std::endl;
}
void test::FracTest() {
	sfrac f1(2, 3);
	sfrac f2(7.0/13.0);
	sfrac f3("123/456");
	sfrac f4;
	f1 = f1.reciprocal();
	std::cout << f1 << std::endl;
	f4 = f1 - 1;
	std::cout << f4 << std::endl;
	f3 += f1;
	std::cout << f2 << std::endl;
	std::cout << f4 << std::endl;
	std::cout << f2 * f3 << std::endl;
	std::cout << f2 / f4 << std::endl;
	std::cout << (f1+f2+f3+f4).rounded(2) << std::endl;
}
void test::ComplexTest() {
	scomp c1(1.f, 1.f);
	scomp c2("1.2345-6.789i");
	scomp c3 = c1 - c2;
	std::cout << c1 + c2 << std::endl;
	std::cout << c2 * c3 << std::endl;
	std::cout << c2 / c3 << std::endl;
	std::cout << c3.conjugated() << std::endl;
	std::cout << c1.absolute() << std::endl;
	std::cout << c1.argument() << std::endl;
	std::cout << (c1 / c3).rounded(2) << std::endl;
}
void test::LinearAlgebraTest() {}
void test::GeometryTest() {}
void test::StatisticTest() {}

#endif