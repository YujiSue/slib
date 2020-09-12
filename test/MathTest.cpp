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
	sfrac f2(7.0 / 13.0);
	sfrac f3("123/456");
	sfrac f4(0.3333333);
	f1 = f1.reciprocal();
	std::cout << f1 << std::endl;
	f4 *= (f1 - 3);
	std::cout << f4 << std::endl;
	f3 += f1;
	std::cout << f2 << std::endl;
	std::cout << f3 << std::endl;
	std::cout << f2 * f3 << std::endl;
	std::cout << f2 / f4 << std::endl;
	std::cout << (-f1 + f2 - f3 + f4).rounded(2) << std::endl;
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

extern inline void openUniKnot(sint d, svecd& knot) {
	auto it = knot.begin();
	sforin(i, 0, d + 1) { E_ = 0.0; NEXT_; }
	auto len = knot.size() - 2 * (d + 1);
	double diff = 1.0 / len;
	sforin(i, 0, len - 1) { E_ = E_PREV + diff; NEXT_; }
	sforin(i, 0, d + 1) { E_ = 1.0; NEXT_; }
	std::cout << knot << std::endl;
}
extern inline double BSplineBasis(sint i, sint dim, double k, svecd& knot) {
	if (dim) {
		double d = 0;
		if (knot[i + dim] != knot[i]) d += (k - knot[i]) / (knot[i + dim] - knot[i]) * BSplineBasis(i, dim - 1, k, knot);
		if (knot[i + dim + 1] != knot[i + 1]) d += (knot[i + dim + 1] - k) / (knot[i + dim + 1] - knot[i + 1]) * BSplineBasis(i + 1, dim - 1, k, knot);
		return d;
	}
	else {
		if (k < knot[i] || knot[i + 1] <= k) return 0;
		else return 1;
	}
}
void test::GeometryTest() {
	std::cout << sgeom::midpoint(v2d(0.0, 0.0), v2d(4.5, 10.5)) << std::endl;
	std::cout << sgeom::length(v2d(2.0, 3.0)) << std::endl;
	std::cout << sgeom::distance(v2d(1.0, 1.0), v2d(2.0, 2.0)) << std::endl;
	std::cout << sgeom::slope(v2d(-3.0, 2.0)) << std::endl;
	std::cout << smath::rad2deg(sgeom::argument(v2d(cos(smath::PI / 3.0), sin(smath::PI / 3.0)))) << std::endl;
	std::cout << sgeom::direction(v2d(1.0, 0.0), v2d(0.0, 1.0)) << std::endl;
	v2d v21(12.3, 45.6);
	sgeom::normalize(v21);
	std::cout << v21 << std::endl;
	std::cout << sgeom::xy2polar(v2d(1.0, 1.0)) << std::endl;
	std::cout << sgeom::polar2xy(v2d(1.0, 2 * smath::PI / 5.0)) << std::endl;
	std::cout << sgeom::trans(v2d(0.0, 0.0), v2d(-2.0, -5.0)) << std::endl;
	std::cout << sgeom::scale(v2d(1.0, 2.0), v2d(0.5, 2.0)) << std::endl;
	std::cout << sgeom::rotate(v2d(1.0, 1.0), smath::PI / 2.0) << std::endl;
	std::cout << sgeom::skew(v2d(1.0, 1.0), 2.0, HORIZONTAL) << std::endl;
	std::cout << sgeom::skew(v2d(1.0, 1.0), 3.0, VERTICAL) << std::endl;

	std::cout << sgeom::length(v3d(1.0, 2.0, 3.0)) << std::endl;
	std::cout << sgeom::distance(v3d(1.0, 1.0, 1.0), v3d(2.0, 2.0, 2.0)) << std::endl;
	std::cout << sgeom::azimuth(v3d(-3.0, 2.0, 0.0)) << std::endl;
	std::cout << sgeom::altitude(v3d(-1.0, 2.0, 2.0)) << std::endl;
	std::cout << sgeom::direction(v3d(1.0, 2.0, 3.0), v3d(3.0, 2.0, 1.0)) << std::endl;
	v3d v31(12.3, 45.6, 78.9);
	sgeom::normalize(v31);
	std::cout << v31 << std::endl;
	std::cout << sgeom::xyz2sphere(v3d(1.0, 1.0, 1.0)) << std::endl;
	std::cout << sgeom::sphere2xyz(v3d(1.0, 2 * smath::PI / 5.0, smath::PI / 2.0)) << std::endl;
	std::cout << sgeom::xyz2tube(v3d(1.0, 1.0, 1.0)) << std::endl;
	std::cout << sgeom::tube2xy(v3d(1.0, 2 * smath::PI / 5.0, 0.5)) << std::endl;

	std::cout << sgeom::length(v4d(1.0, 2.0, 3.0, 4.0)) << std::endl;
	std::cout << sgeom::distance(v4d(1.0, 1.0, 1.0, 1.0), v4d(2.0, 2.0, 2.0, 2.0)) << std::endl;
	v4d v41(12.3, 45.6, 78.9, 10.0);
	sgeom::normalize(v41);
	std::cout << v41 << std::endl;

	std::cout << sgeom::center(saread(1.0, 2.0, 3.0, 4.0)) << std::endl;
	svecd vd1({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 });
	std::cout << sgeom::length(vd1) << std::endl;
	std::cout << sgeom::distance(vd1, svecd({ -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0 })) << std::endl;
	std::cout << sgeom::lpnorm(vd1) << std::endl;
	sgeom::normalize(vd1);
	std::cout << vd1 << std::endl;

	smath::SRandom rand;
	v2dvec v2d1;
	sforin(i, 0, 100) {
		auto r = 1.0 + rand.rnorm(1.0, 0.1);
		v2d1.add(r * cos(i * smath::PI / 50.0), r * sin(i * smath::PI / 50.0));
	}
	std::cout << sgeom::length(v2d1) << std::endl;
	v2dvec c1, c2, c3, c4;
	sgeom::bezier2(v2d(0.0, 0.0), v2d(0.5, 1.0), v2d(1.0, 0.0), c1);
	std::cout << c1 << std::endl;
	sgeom::bezier3(v2d(0.0, 0.0), v2d(0.25, 1.0), v2d(0.75, 1.0), v2d(1.0, 0.0), c2);
	std::cout << c2 << std::endl;
	std::cout << v2d1 << std::endl;
	sgeom::bspline2(v2d1, c3);
	std::cout << c3 << std::endl;
	sgeom::bspline3(v2d1, c4);
	std::cout << c4 << std::endl;

}
void test::StatisticTest() {
	svecd v1({ 32, 40, 21, 80, 50, 48, 74, 20, 59, 68 }), v2, v3;
	std::cout << sstat::argmax(v1) << std::endl;
	std::cout << sstat::maxv(v1) << std::endl;
	std::cout << sstat::argmin(v1) << std::endl;
	std::cout << sstat::minv(v1) << std::endl;
	std::cout << sstat::sum(v1) << std::endl;
	std::cout << sstat::average(v1) << std::endl;
	std::cout << sstat::angaverage(v1) << std::endl;
	std::cout << sstat::product(v1) << std::endl;
	sstat::difference(v1, v2);
	std::cout << v2 << std::endl;
	sstat::maverage(v1, v3, 3);
	std::cout << sstat::variance(v1) << std::endl;


	std::cout << sstat::stddev(v1) << std::endl;
	std::cout << sstat::skew(v1) << std::endl;
	std::cout << sstat::kurtosis(v1) << std::endl;
	std::cout << sstat::median(v1) << std::endl;
	std::cout << sstat::quartile(v1) << std::endl;
	std::cout << sstat::count<double>(v1, [](const double& v) { if (v < 50) return true; else return false; }) << std::endl;
	std::cout << sstat::covariance(svecd({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}), svecd({10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0}));



	/*
	template<typename T>
	extern inline sla::SMatrix<T, CMemory<T>> covmat(const sla::SVector<sla::SVector<T, CMemory<T>>> & vec) {
			auto sum = sstat::sum(vec);
			auto count = vec.size();
			auto dim = vec[0].size();
			svecd ave(dim, 0), diff(dim);
			sforeach2(vec, ave) E2_ = (double)E1_ / count;
			smat<T, CMemory<T>> mat(dim, dim, 0);
			double dx, dy, dz, dw;
			sforeach(vec) {
					sforin(d, 0, dim) diff[d] = (double)E_[d] - ave[d];
					sforin(i, 0, dim) {
							sforin(j, 0, i + 1) mat[i][j] += diff[i] * diff[j];
					}
			}
			sforin(i, 0, dim) {
					sforin(j, i + 1, dim) mat[i][j] = mat[j][i];
			}
			return mat / dim;
	}
	*/

	/*
	extern inline std::function<double(double)> normFunc(double m = 0.0, double s = 1.0) {
			return [m, s](double x) {
					return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
			};
	}
	extern inline double norm(double x, double m = 0.0, double s = 1.0) {
			return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
	}
	extern inline double pnorm(double x, double m = 0.0, double s = 1.0);
	extern inline double qnorm(double x, double m = 0.0, double s = 1.0);

	extern inline std::function<double(double)> chisqFunc(double x, double n);
	extern inline double chisq(double x, double n);
	extern inline double pchisq(double x, double n);
	extern inline double qchisq(double x, double n);
	extern inline std::function<double(double)> binomFunc(double m = 0.0, double s = 1.0) {
			return [m, s](double x) {
					return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
			};
	}
	extern inline double pbinom(int x, int n, double p) {

	}


	extern inline std::function<double(int)> poisFunc(double l) {
			return [l](int x) {
					return (pow(l, x) * exp(-l)) / smath::factorial(x);
			};
	}
	extern inline double pois(int x, double l) { return (pow(l, x) * exp(-l)) / smath::factorial(x); }
	extern inline double ppois(int x, double l);
	extern inline double qpois(int x, double l);

	extern inline double gammaFunc(double d);
	extern inline double ibetaFunc(double d, double m, double n);
	extern inline double betaFunc(double m, double n);
	*/
}

#endif