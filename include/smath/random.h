#ifndef SMATH_RANDOM_H
#define SMATH_RANDOM_H
#include "smath/mbasic.h"
#include "sbasic/array.h"
namespace slib {
	namespace smath {
		/**
		* @class Random
		* \~english @brief Random generator class
		* \~japanese @brief 乱数生成クラス
		*/
		class SLIB_DLL Random {
			std::random_device _seed;
			std::mt19937 _engine;

		public:
			Random();
			~Random();

			int iruni(int beg, int end);
			double runi(double beg = 0.0, double end = 1.0);
			double rnorm(double m = 0.0, double s = 1.0);
			double rlognorm(double m = 0.0, double s = 1.0);
			double rchisq(double n);
			int rpois(double m);
			double rexp(double l);
			double rgamma(double a, double b);
			double rweib(double a, double b);
			double rbern(double p);
			double rbinom(int n, double p);
			double rgeom(double p);
			double rnbinom(int n, double p);
			double rrayleigh(double s);
			double rvonmises(double m, double k);
			template<typename T>
			intarray sampling(const Array<T>& array, const size_t count, bool repeat=false) {
				intarray samples;
				std::uniform_int_distribution<> rand(0, array.size());
				int i;
				while (samples.size() < count) {
					i = rand(_engine);
					if (samples.indexOf(i) == NOT_FOUND) samples.add(i);
					samples.sort();
				}
				return samples;
			}
		};
	}
}
#endif