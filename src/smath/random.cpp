#include "smath/random.h"
#include "smath/calc.h"

slib::smath::Random::Random() { _engine = std::mt19937(_seed()); }
slib::smath::Random::~Random() {}
int slib::smath::Random::iruni(int beg, int end) {
	std::uniform_int_distribution<> dist(beg, end); return dist(_engine);
}
double slib::smath::Random::runi(double beg, double end) {
	std::uniform_real_distribution<> dist(beg, end); return dist(_engine);
}
double slib::smath::Random::rnorm(double m, double s) {
	std::normal_distribution<> dist(m, s); return dist(_engine);
}
double slib::smath::Random::rlognorm(double m, double s) {
	std::lognormal_distribution<> dist(m, s); return dist(_engine);
}
double slib::smath::Random::rchisq(double n) {
	std::chi_squared_distribution<> dist(n); return dist(_engine);
}
int slib::smath::Random::rpois(double m) {
	std::poisson_distribution<> dist(m); return dist(_engine);
}
double slib::smath::Random::rexp(double l) {
	std::exponential_distribution<> dist(l); return dist(_engine);
}
double slib::smath::Random::rgamma(double a, double b) {
	std::gamma_distribution<> dist(a, b); return dist(_engine);
}
double slib::smath::Random::rweib(double a, double b) {
	std::weibull_distribution<> dist(a, b); return dist(_engine);
}
double slib::smath::Random::rbern(double p) {
	std::bernoulli_distribution dist(p); return dist(_engine);
}
double slib::smath::Random::rbinom(int n, double p) {
	std::binomial_distribution<> dist(n, p); return dist(_engine);
}
double slib::smath::Random::rgeom(double p) {
	std::geometric_distribution<> dist(p); return dist(_engine);
}
double slib::smath::Random::rnbinom(int n, double p) {
	std::negative_binomial_distribution<> dist(n, p); return dist(_engine);
}
double slib::smath::Random::rrayleigh(double s) {
	double r = runi(); return s * sqrt(-2.0 * log(r));
}
double slib::smath::Random::rvonmises(double m, double k) {
	double r = 1 + std::pow((1 + 4 * k * k), 0.5);
	double rho = 0.5 * (r - std::pow(2 * r, 0.5)) / k;
	double s = 0.5 * (1 + rho * rho) / rho;
	bool b = false;
	double phase;
	while (!b) {
		double c = std::cos(slib::snum::PI * runi());
		phase = (1 + s * c) / (s + c);
		double tmp = k * (s - phase);
		double r1 = runi();
		b = tmp * (2 - tmp) - r1 > 0;
		if (!b) b = std::log(tmp / r1) + 1 - tmp >= 0;
	}
	double r2 = runi() - 0.5;
	double sig = ((r2 >= 0) - (r2 <= 0));
	return sig * std::acos(phase) + std::fmod(std::fmod(m, 2 * snum::PI) + 2 * snum::PI, 2 * snum::PI);
}