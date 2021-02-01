#include "sbasic/string.h"
#include "smath/mathbasic.h"
#include "smath/calc.h"

slib::smath::SMathException::SMathException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) :
	slib::SException(f, l, func, e, target, note) {
	prefix = "smath";
	switch (err) {
	case DIV_ZERO_ERR:
	{
		message = "Divided by zero.";
		description = TARGET_TEXT(target ? target : "") + u8" is zero.";
		break;
	}
	case UNSIGNED_MINUS_ERR:
	{
		message = "Unsigned value error.";
		description = TARGET_TEXT(target ? target : "") + u8" should be positive.";
		break;
	}
	case DIMENSION_SIZE_ERR:
	{
		message = "Dimension size error.";
		description = TARGET_TEXT(target ? target : "") + u8" " + (note ? note : "");
		break;
	}
	default:
		break;
	}
}
slib::smath::SMathException::~SMathException() {}
bool slib::smath::isInteger(const float& f) { return f == floorf(f) || f == ceilf(f); }
bool slib::smath::isInteger(const double& d) { return d == floor(d) || d == ceil(d); }
double slib::smath::rootN(double val, size_t n) {
	if (n == 0) return 1.0;
	else if (n == 1) return val;
	double r = pow(val, 1.0 / n);
	return (r * (n - 1) + val / (pow(r, n - 1))) / n;
}
size_t slib::smath::digit(int i) {
	std::string str = std::to_string(i);
	return str.length() - (i < 0 ? 1 : 0);
}
size_t slib::smath::decim(double d) {
	std::string str = std::to_string(d);
	size_t pos = str.find(".");
	return pos == std::string::npos ? str.length() : pos;
}
sinteger slib::smath::gcd(sinteger n1, sinteger n2) {
	sinteger ori = n1, div = n2, res = n1 % n2;
	while (res) { ori = div; div = res; res = ori % div; }
	return div;
}
sinteger slib::smath::lcm(sinteger n1, sinteger n2) { return n1 * n2 / gcd(n1, n2); }
sinteger slib::smath::factorial(sinteger n, sinteger k) {
	sinteger f = 1;
	sforin(i, 0, k) { f *= n; n--; }
	return f;
}
sinteger slib::smath::factorial(sinteger n) { return factorial(n, n); }
sinteger slib::smath::combination(sinteger n, sinteger k) {
	if (k <= n - k) {
		std::pair<sinteger, sinteger> frac(1, 1);
		while (1 < k) {
			frac.first *= n;
			frac.second *= k;
			reduction(frac.first, frac.second);
			--n; --k;
		}
		return frac.first;
	}
	else return combination(n, n - k);
}

double slib::smath::deg2rad(double deg) { return deg * smath::PI / 180.0; }
double slib::smath::rad2deg(double rad) { return rad * 180.0 / smath::PI; }
slib::smath::SRandom::SRandom() {
	_engine = std::mt19937(_seed());
}
slib::smath::SRandom::~SRandom() {}

sint slib::smath::SRandom::iruni(int beg, int end) {
	std::uniform_int_distribution<> dist(beg, end);
	return dist(_engine);
}
double slib::smath::SRandom::runi(double beg, double end) {
	std::uniform_real_distribution<> dist(beg, end);
	return dist(_engine);
}
double slib::smath::SRandom::rnorm(double m, double s) {
	std::normal_distribution<> dist(m, s);
	return dist(_engine);
}
double slib::smath::SRandom::rlognorm(double m, double s) {
	std::lognormal_distribution<> dist(m, s);
	return dist(_engine);
}
double slib::smath::SRandom::rchisq(double n) {
	std::chi_squared_distribution<> dist(n);
	return dist(_engine);
}
int slib::smath::SRandom::rpois(double m) {
	std::poisson_distribution<> dist(m);
	return dist(_engine);
}
double slib::smath::SRandom::rexp(double l) {
	std::exponential_distribution<> dist(l);
	return dist(_engine);
}
double slib::smath::SRandom::rgamma(double a, double b) {
	std::gamma_distribution<> dist(a, b);
	return dist(_engine);
}
double slib::smath::SRandom::rweib(double a, double b) {
	std::weibull_distribution<> dist(a, b);
	return dist(_engine);
}
double slib::smath::SRandom::rbern(double p) {
	std::bernoulli_distribution dist(p);
	return dist(_engine);
}
double slib::smath::SRandom::rbinom(int n, double p) {
	std::binomial_distribution<> dist(n, p);
	return dist(_engine);
}
double slib::smath::SRandom::rgeom(double p) {
	std::geometric_distribution<> dist(p);
	return dist(_engine);
}
double slib::smath::SRandom::rnbinom(int n, double p) {
	std::negative_binomial_distribution<> dist(n, p);
	return dist(_engine);
}

double slib::smath::SRandom::rrayleigh(double s) {
	double r = runi();
	return s * sqrt(-2.0 * log(r));
}
double slib::smath::SRandom::rvonmises(double m, double k) {
	double r = 1 + std::pow((1 + 4 * k * k), 0.5);
	double rho = 0.5 * (r - std::pow(2 * r, 0.5)) / k;
	double s = 0.5 * (1 + rho * rho) / rho;
	bool b = false;
	double phase;
	while (!b) {
		double c = std::cos(smath::PI * runi());
		phase = (1 + s * c) / (s + c);
		double tmp = k * (s - phase);
		double r1 = runi();
		b = tmp * (2 - tmp) - r1 > 0;
		if (!b) b = std::log(tmp / r1) + 1 - tmp >= 0;
	}
	double r2 = runi() - 0.5;
	double sig = ((r2 >= 0) - (r2 <= 0));
	return sig * std::acos(phase) + std::fmod(std::fmod(m, 2 * smath::PI) + 2 * smath::PI, 2 * smath::PI);
}
