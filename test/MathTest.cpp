#include "Test.h"
#include "smath/smath.h"

using namespace slib;

#ifdef MATH_TEST
using namespace slib::smath;

void test::CalcTest() {
	std::cout << "1.0f==1?" << (smath::isInteger(1.0f) ? "true" : "false") << std::endl;
	std::cout << "1.5==1 or 2?" << (smath::isInteger(1.5) ? "true" : "false") << std::endl;
	std::cout << "3//2=" << smath::quot(3, 2) << std::endl;
	std::cout << "-5//2=" << smath::quot(-5, 2) << std::endl;
	std::cout << "4.5//2=" << smath::quot(4.5, 2) << std::endl;
	std::cout << "2^3=" << smath::power(2, 3) << std::endl;
	std::cout << "1.2f^2=" << smath::power(1.2f, 2) << std::endl;
	std::cout << "2.5^4=" << smath::power(2.5, 4) << std::endl;
	std::cout << "2.56^(1/2)=" << smath::rootN(2.56, 2) << std::endl;
	std::cout << "8^(1/3)=" << smath::rootN(8, 3) << std::endl;
	std::cout << "digit of 1234567=" << smath::digit(1234567) << std::endl;
	std::cout << "decim of 123.4567=" << smath::decim(123.4567) << std::endl;
	std::cout << "G.C.D.(16, 36)=" << smath::gcd(16, 36) << std::endl;
	std::cout << "L.C.M.(24, 56)=" << smath::lcm(24, 56) << std::endl;
	std::cout << "factorial(4, 2)=" << smath::factorial(4, 2) << std::endl;
	std::cout << "combination(6, 2)=" << smath::combination(6, 2) << std::endl;
	std::cout << "kronecker delta(1, 1)=" << smath::kdelta(1, 1) << std::endl;
	std::cout << "radian of 45=" << smath::deg2rad(45) << std::endl;
	std::cout << "degree of 1.57=" << smath::rad2deg(1.57) << std::endl;
	std::cout << "sigmoid(1.2)=" << smath::sigmoid(1.2) << std::endl;
	std::cout << "splus(0.7)=" << smath::splus(0.7) << std::endl;
	std::cout << "relu(-0.3)=" << smath::relu(-0.3) << std::endl;
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
}
void test::ComplexTest() {
}
void test::LinearAlgebraTest() {
	v2i v1(1, 1);

}
void test::GeometryTest() {}
void test::StatisticTest() {}

#endif