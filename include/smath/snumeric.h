#ifndef SMATH_NUMERIC_H
#define SMATH_NUMERIC_H

#include "smath/calc.h"
#include "smath/la.h"

namespace slib {
    namespace smath {
        extern inline double legendre(double d, size_t n) {
			if (!n) return 1.0;
			if (n == 1) return d;
			v3d val = { 1.0, d, (3.0 * d * d - 1.0) / 2.0 };
			for (int i = 3; i <= n; ++i) {
				val.x = val.y; val.y = val.z;
				val.z = (d * val.y * (2 * i - 1) - val.x * (i - 1)) / i;
			}
			return val.z;
		}
        extern inline double laguerre(double d, size_t n) {
			if (!n) return 1.0;
			if (n == 1) return 1.0 - d;
			v3d val = { 1.0, 1.0 - d, (3.0 - d) * (1 - d) - 1 };
			for (int i = 3; i <= n; ++i) {
				val.x = val.y; val.y = val.z;
				val.z = val.y * (2.0 * i - 1.0 - d) - val.x * (i - 1) * (i - 1);
			}
			return val.z;
		}

		extern const Array<svecd> GLGD_QUADR_WX = {
			{ 1.0, -0.57735026918962576451, 1.0, 0.57735026918962576451 },
			{ 0.55555555555555555552, -0.77459666924148337704, 0.88888888888888888889, 0, 0.55555555555555555552, 0.77459666924148337704 },
			{ 0.34785484513745385742, -0.86113631159405257522, 0.65214515486254614263, -0.3399810435848562648,
			  0.65214515486254614263, 0.3399810435848562648, 0.34785484513745385742, 0.86113631159405257522 },
			{ 0.23692688505618908748, -0.9061798459386639928, 0.47862867049936646803, -0.53846931010568309104, 0.56888888888888888889, 0,
			  0.47862867049936646803, 0.53846931010568309104, 0.23692688505618908748, 0.9061798459386639928 },
			{ 0.17132449237917034508, -0.93246951420315202781, 0.36076157304813860758, -0.66120938646626451366, 0.46791393457269104739, -0.23861918608319690863,
			  0.46791393457269104739, 0.23861918608319690863, 0.36076157304813860758, 0.66120938646626451366, 0.17132449237917034508, 0.93246951420315202781 },
			{ 0.1294849661688696932, -0.94910791234275852453, 0.27970539148927666793, -0.74153118559939443986, 0.38183005050511894494, -0.40584515137739716691,
			  0.41795918367346938776, 0, 0.38183005050511894494, 0.40584515137739716691, 0.27970539148927666793, 0.74153118559939443986, 0.1294849661688696932, 0.94910791234275852453 }
		};
		extern const Array<svecd> GLGR_QUADR_WX = {
			{ 0.85355339059327376219, 0.5857864376269049512, 0.1464466094067262378, 3.4142135623730950488 },
			{ 0.71109300992917301547, 0.41577455678347908331, 0.27851773356924084882, 2.2942803602790417198, 0.010389256501586135749, 6.2899450829374791969 },
			{ 0.60315410434163360164, 0.3225476896193923118, 0.35741869243779968662, 1.7457611011583465757,
			  0.038887908515005384271, 4.5366202969211279833, 0.00053929470556132745012, 9.3950709123011331292 },
			{ 0.52175561058280865254, 0.2635603197181409102, 0.39866681108317592751, 1.4134030591065167922, 0.075942449681707595393, 3.5964257710407220812,
			  0.0036117586799220484546, 7.0858100058588375569, 0.0000233699723857762279, 12.640800844275782659 },
			{ 0.45896467394996359368, 0.22284660417926068946, 0.41700083077212099431, 1.1889321016726230307, 0.11337338207404497573, 2.9927363260593140777,
			  0.0103991974531490749, 5.7751435691045105018, 0.00026101720281493205948, 9.8374674183825899177, 0.0000008985479064296212385, 15.982873980601701783 },
			{ 0.40931895170127390207, 0.19304367656036241384, 0.42183127786171978021, 1.0266648953391919503, 0.1471263486575052784, 2.5678767449507462069, 0.020633514468716939866, 4.9003530845264845681,
			  0.0010740101432807455221, 8.1821534445628607911, 0.000015865464348564201269, 12.734180291797813758, 0.000000031703154789955805616, 19.395727862262540312 }
		};
        extern inline double gaussIntegral(std::function<double(double)> &func, const svecd &vec) {
			double val = 0.0;
			sforeach(vec) { val += E_ * func(E_NXT); NEXT_; }
			return val;
		}
        extern inline double glgdIntegral(std::function<double(double)> &func,
                                   int n = 7, const sranged &range = sranged(-1.0, 1.0)) {
			if (n == 1) return 0.0;
			if (2 <= n && n <= 7) {
				if (range.begin == -1.0 && range.end == 1.0)
					return gaussIntegral(func, GLGD_QUADR_WX[n - 2]);
				else {
					auto vec = GLGD_QUADR_WX[n - 2];
					sforin(i, 0, n)
						vec[2*i+1] = vec[2*i+1] * (range.end - range.begin) / 2.0 + (range.begin + range.end) / 2.0;
					return (range.end - range.begin) / 2.0 * gaussIntegral(func, vec);
				}
			}
			else {
				svecd vec(2*n);
				for (int i = 0; i < n; ++i) {
					vec[2 * i + 1] = cos((0.75 + i) / (0.5 + n) * smath::PI);
					vec[2 * i] = vec[2 * i + 1] * (range.end - range.begin) / 2.0 + (range.end + range.begin) / 2.0;
					auto tmp = legendre(vec[2 * i + 1], n - 1) * n;
					vec[2 * i] = (2.0 * (1.0 - vec[2 * i + 1] * vec[2 * i + 1])) / (tmp * tmp);
				}
				return (range.end - range.begin) / 2.0 * gaussIntegral(func, vec);
			}
		}
        extern inline double glgrIntegral(std::function<double(double)> &func, int n = 7) {
			if (n == 1) return 0.0;
			if (2 <= n && n <= 7)
				return gaussIntegral(func, GLGR_QUADR_WX[n - 2]);
			else {
				svecd vec(2*n);
				for (int i = 0; i < n; ++i) {
					vec[2 * i + 1] = (smath::PI * smath::PI * (0.75 + i) * (0.75 + i)) / (4.0 * n);
					auto tmp = laguerre(vec[2 * i + 1], n + 1);
					vec[2 * i] = (vec[2 * i + 1] * smath::power(smath::factorial(n), 2)) / (tmp * tmp);
				}
				return gaussIntegral(func, vec);
			}
		}
        extern inline double mcIntegral(std::function<double(double)> &func,
                                 const sranged &border, const sranged &range, int repeat) {
			SRandom rand1, rand2;
			int count = 0;
			for (int i = 0; i < repeat; ++i) {
				double mcval = rand1.runi(border.begin, border.end),
					val = func(rand2.runi(range.begin, range.end));
				if (val < 0.0) {
					if (mcval < 0.0 && val <= mcval) ++count;
				}
				else {
					if (0.0 <= mcval && mcval <= val) ++count;
				}
			}
			return (double)count / repeat * (range.end - range.begin) * (border.end - border.begin);
		}
        
		template<typename Quant>
		extern inline void euler(Quant* cur, Quant* nxt,
			std::function<Quant(double, Quant*)>* func,
			double* t, double* dt) {
			*nxt = (*cur) + (*func)(*t, cur) * (*dt);
			*t += *dt;
		}
		template<typename Quant>
		extern inline void euler2(Quant* cur, Quant* nxt,
			std::function<Quant(double, Quant*)>* func,
			double* t, double* dt) {
			Quant tmp = (*func)(*t, cur) * (*dt) * 0.5f + (*cur);
			*nxt = (*cur) + (*func)((*t) + (*dt) * 0.5f, cur) * (*dt);
			*t += *dt;
		}
		template<typename Quant>
		extern inline void heun(Quant* cur, Quant* nxt,
			std::function<Quant(double, Quant*)>* func,
			double* t, double* dt) {
			Quant predic;
			euler(cur, &predic, func, t, dt); *t -= *dt;
			*nxt = (*cur) + ((*func)(*t, cur) + (*func)((*t) + (*dt), &predic)) * (*dt) * 0.5;
			*t += *dt;
		}
		template<typename Quant, typename Diff>
		extern inline void leapfrog(Quant* cur, Diff* curdiff,
			Quant* nxt, Diff* nxtdiff,
			std::function<Diff & (Quant*)>* diff,
			std::function<Diff(Quant*)>* func,
			float* t, float* dt) {
			*nxtdiff = (*curdiff) + (*func)(cur) * (*dt);
			*nxt = (*cur) + (*nxtdiff) * (*dt);
			(*diff)(cur) = (*curdiff) + (*func)(cur) * (*dt) * 0.5f;
			*t += *dt;
		}
		template<typename Quant, typename Diff>
		extern inline void rk4(Quant* cur, Quant* nxt, Diff** buf,
			std::function<Diff(float, Quant*)>* func,
			float* t, float* dt) {
			Quant tmp;
			float dt_ = (*dt) * 0.5f, t_ = (*t) + dt_;
			*buf[0] = (*func)(*t, cur);
			tmp = (*cur) + (*buf[0]) * dt_;
			*buf[1] = (*func)(t_, &tmp);
			tmp = (*cur) + (*buf[1]) * dt_;
			*buf[2] = (*func)(t_, &tmp);
			tmp = (*cur) + (*buf[2]) * (*dt);
			*buf[3] = (*func)((*t) + (*dt), &tmp);
			*nxt = (*cur) + ((*buf[0]) + 2.f * (*buf[1]) + 2.f * (*buf[2]) + (*buf[3])) * (*dt) / 6.f;
			*t += *dt;
		}
    }
}


#endif
