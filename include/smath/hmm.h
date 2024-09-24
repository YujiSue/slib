#ifndef SMATH_SIM_H
#define SMATH_SIM_H
#include "smath/mbasic.h"
#include "smath/calc.h"
#include "smath/matrix.h"
#include "sobj/sarray.h"
#include "sio/stream.h"

namespace slib {
	/**
	* @cond
	*/
	namespace smath {
		
		template<typename T>
		void baumwelch_e(sveci& observation, svecf& initial, smatf& transition, smatf& emission,
			Vector<svecf>& alpha, Vector<svecf>& beta, Vector<svecf>& gamma, Vector<smatf>& xi) {
			int length = (int)observation.size();
			int nstate = (int)emission.row;
			int nobs = (int)emission.col;
			sforin(i, 0, nstate) {
				alpha[0][i] = initial[i] * emission[i][observation[0]];
				beta[length - 1][i] = 1;
			}
			sforin(t, 1, length) {
				sforin(i, 0, nstate) {
					alpha[t][i] = 0;
					sforin(j, 0, nstate) alpha[t][i] += alpha[t - 1][j] * transition[j][i];
					alpha[t][i] *= emission[i][observation[t]];
				}
			}
			srforin(t, 0, length - 1) {
				sforin(i, 0, nstate) {
					beta[t][i] = 0;
					sforin(j, 0, nstate) beta[t][i] += beta[t + 1][j] * transition[i][j] * emission[j][observation[t + 1]];
				}
			}
			sforin(t, 0, length) {
				auto sg = 0.f, sx = 0.f;
				sforin(i, 0, nstate) {
					sg += alpha[t][i] * beta[t][i];
					if (t == length - 1) break;
					sforin(j, 0, nstate) {
						sx += alpha[t][i] * transition[i][j] * beta[t + 1][j] * emission[j][observation[t + 1]];
					}
				}
				sforin(i, 0, nstate) gamma[t][i] = alpha[t][i] * beta[t][i] / sg;
				if (t == length - 1) break;
				sforin(i, 0, nstate) {
					sforin(j, 0, nstate) {
						xi[t][i][j] = alpha[t][i] * transition[i][j] * beta[t + 1][j] * emission[j][observation[t + 1]] / sx;
					}
				}
			}
		}
		template<typename T>
		void baumwelch_m(sveci& observation, svecf& initial, smatf& transition, smatf& emission,
			Vector<svecf>& alpha, Vector<svecf>& beta, Vector<svecf>& gamma, Vector<smatf>& xi) {
			int length = (int)observation.size();
			int nstate = (int)emission.row;
			int nobs = (int)emission.col;
			sforin(i, 0, nstate) {
				auto sg = 0.f, so = 0.f;
				initial[i] = gamma[0][i];
				sforin(j, 0, nstate) transition[i][j] = 0;
				sforin(k, 0, nobs) emission[i][k] = 0;
				sforin(t, 0, length - 1) {
					sg += gamma[t][i];
					sforin(j, 0, nstate) transition[i][j] += xi[t][i][j];
					sforin(k, 0, nobs) emission[i][k] += gamma[t][i] * (observation[t] == k ? 1 : 0);
				}
				sforin(j, 0, nstate) transition[i][j] /= sg;
				sg += gamma[length - 1][i];
				sforin(k, 0, nobs) {
					emission[i][k] += gamma[length - 1][i] * (observation[length - 1] == k ? 1 : 0);
					emission[i][k] /= sg;
				}
			}
		}

		template<typename T>
		extern void baumwelch(
			Vector<int>& observation,
			Vector<T>& initial,
			Matrix<T>& transition,
			Matrix<T>& emission,
			int epoch
		) {
			int length = (int)observation.size();
			int nstate = (int)transition.row;
			//
			Vector<svecf> alpha(length);
			Vector<svecf> beta(length);
			Vector<svecf> gamma(length);
			Vector<smatf> xi(length);
			sforin(t, 0, length) {
				alpha[t].resize(nstate, 0.f);
				beta[t].resize(nstate, 0.f);
				gamma[t].resize(nstate, 0.f);
				xi[t].resize(nstate, nstate);
			}
			//
			sforin(e, 0, epoch) {
				SPrint("Epoch : ", (e + 1), "/", epoch);
				baumwelch_e(observation, initial, transition, emission, alpha, beta, gamma, xi);
				baumwelch_m(observation, initial, transition, emission, alpha, beta, gamma, xi);
			}
		}

		template<typename T>
		extern void viterbi(
			Vector<int>& observation,
			Vector<int>& states,
			Vector<T>& initial,
			Matrix<T>& transition,
			Matrix<T>& emission
		) {
			// Data length
			int length = (int)observation.size();
			// Num. of states
			int num = (int)emission.row;
			//
			states.resize(length);
			Matrix<T> probs(length, num);
			probs.reset(-std::numeric_limits<T>::infinity());
			Matrix<int> buf(length, num);
			//
			sforin(s, 0, num) {
				probs[0][s] = std::log(initial[s]) + std::log(emission[s][observation[0]]);
			}
			sforin(t, 1, length) {
				sforin(s, 0, num) {
					sforin(r, 0, num) {
						auto p = probs[t - 1][r] + std::log(transition[r][s]) + std::log(emission[s][observation[t]]);
						if (probs[t][s] < p) {
							probs[t][s] = p;
							buf[t][s] = r;
						}
					}
				}
			}
			//
			auto max = probs[length - 1][0]; auto maxi = 0;
			sforin(s, 1, num) {
				if (max < probs[length - 1][s]) {
					max = probs[length - 1][s];
					maxi = s;
				}
			}
			states[-1] = maxi;
			srforin(t, 0, length - 1) states[t] = buf[t + 1][states[t + 1]];
		}
		
		template<typename T>
		class HmmModel {
		public:
			int epoch, batch;
			Vector<T> initial;
			Matrix<T> transition;
			Matrix<T> emission;
		public:
			HmmModel() { epoch = 1; batch = 1; }
			~HmmModel() {}

			void init(int nstate, int nobject, int num) {
				initial.resize(nstate);
				transition.resize(nstate, nstate);
				emission.resize(nstate, nobject);
				epoch = num;
			}
			void set(const SArray &trans, const SArray &emit) {
				sfor2(transition, trans) $_1 = (T)$_2.number();
				sfor2(emission, emit) $_1 = (T)$_2.number();
			}
			void fit(const Vector<int>& observation, const Vector<int>& states) {
				if (states.empty()) baumwelch(observation, initial, transition, emission, epoch);
				else {

				}
			}
			void predict(const Vector<int>& observation, Vector<int>& states) {
				auto n = transition.row;
				auto o = observation[0];
				sforin(i, 0, n) initial[i] = emission[i][o];
				viterbi(observation, states, initial, transition, emission);
			}
		};
	}
	/**
	* @endcond
	*/
}
#endif
