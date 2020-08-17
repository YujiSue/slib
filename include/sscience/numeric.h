#ifndef SSCI_NUMERIC_H
#define SSCI_NUMERIC_H

#include "smath/calc.h"
#include "smath/la.h"

namespace slib {
    namespace ssci {
		extern double legendre(double d, size_t n);
		extern double laguerre(double d, size_t n);
		extern svdvec GLGD_QUADR_WX;
		extern svdvec GLGR_QUADR_WX;
		extern double gaussIntegral(std::function<double(double)>& func, const svecd& vec);
		extern double glgdIntegral(std::function<double(double)>& func, sint n = 7, sranged range = sranged(-1.0, 1.0));
		extern double glgrIntegral(std::function<double(double)>& func, sint n = 7);
		extern double mcIntegral(std::function<double(double)>& func, sranged border, sranged range, sint repeat);
		
		template<typename T, class M>
		extern inline void euler(svec<T, M> &current, svec<T, M>& next,
			std::function<void(double, <T, M> &, svec<T, M> &)> &diff,
			double &t, double &dt) {svec
			diff(t, current, next); next *= dt; next += current; t += dt;
		}
		template<typename T, class M>
		extern inline void euler2(svec<T, M>& current, svec<T, M>& next, svec<T, M>& tmp,
			std::function<void(double, svec<T, M>&, svec<T, M>&)>& diff,
			double& t, double& dt) {
			diff(t, current, tmp); tmp *= 0.5 * dt; tmp += current;
			diff(t + 0.5 * dt, next); next *= dt; next += current; t += dt;
		}
		template<typename T, class M>
		extern inline void heun(svec<T, M>& current, svec<T, M>& next,
			svec<T, M>& pred, svec<T, M>& tmp1, svec<T, M>& tmp2,
			std::function<void(double, svec<T, M>&, svec<T, M>&)>& diff,
			double& t, double& dt) {
			euler(current, tmp, diff, t, dt); t -= dt;
			diff(t, current, tmp1); diff(t + dt, pred, tmp2);
			next = current + (tmp1 + tmp2) * dt * 0.5; t += dt;
		}
		template<typename T, class M>
		extern inline void leapfrog(svec<T, M>& current, svec<T, M>& dcurrent, svec<T, M>& ddcurrent,
			svec<T, M>& next, svec<T, M>& dnext,
			std::function<void(double, svec<T, M>&, svec<T, M>&)> &diff,
			double& t, double& dt) {
			diff(t, current, ddcurrent);
			next = current + dcurrent * dt + 0.5 * dt * dt * ddcurrent;
			diff(t, next, dnext);
			dnext = dcurrent + 0.5 * dt * (ddcurrent + dnext);
			t += dt;
		}
		template<typename T, class M>
		extern inline void rk4(svec<T, M>& current, svec<T, M>& next, svec<T, M>* tmp,
			std::function<void(double, svec<T, M>&, svec<T, M>&)> &diff,
			double& t, double& t_, double& dt, double &dt_) {
			t_ = t + dt_;
			diff(t, current, tmp[0]);
			tmp[4] = current + tmp[0] * dt_;
			diff(t_, tmp[4], tmp[1]);
			tmp[4] = current + tmp[1] * dt_;
			diff(t_, tmp[4], tmp[2]);
			tmp[4] = current + tmp[2] * dt;
			diff(t + dt, tmp[4], tmp[3]);
			next = current + (tmp[0] + 2.0 * tmp[1] + 2.0 * tmp[2] + tmp[3]) * dt / 6.0;
			t += dt;
		}

		template<typename T, class M>
		class SNumericalAnalyze {

		public:
			SNumericalAnalyze() {}
			~SNumericalAnalyze() {}





		};

    }
}
#endif
