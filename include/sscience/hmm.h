#ifndef SSCI_HMM_H
#define SSCI_HMM_H

#include "sbasic/array.h"
#include "smath/stat.h"
#include "sscience/ml.h"

namespace slib {
	using namespace smath;
	namespace ssci {

		inline void bweStep(const sveci& observation, svecd &init, svdvec &alpha, svdvec&beta, svdvec& gamma, smdvec &xi, smatd& emission, smatd& transition) {
			auto time = observation.size();
			auto stnum = emission.row;
			auto obsnum = emission.col;
			auto ap = alpha.ptr(), ap_ = alpha.ptr();
			auto op = observation.ptr();
			sforin(i, 0, stnum) (*ap)[i] = init[i] * emission[i][*op]; 
			++ap;
			sforin(t, 1, time) {
				sforin(i, 0, stnum) {
					(*ap)[i] = 0;
					sforin(j, 0, stnum) {
						(*ap)[i] += (*ap_)[j] * transition[j][i];
					}
					(*ap)[i] *= emission[i][*op];
				}
				++ap; ++ap_; ++op;
			}
			auto bp = &beta[-1], bp_ = bp;
			sforin(i, 0, stnum) (*bp)[i] = 1;
			--bp; --op;
			srforin(t, 0, time - 1) {
				sforin(i, 0, stnum) {
					(*bp)[i] = 0;
					sforin(j, 0, stnum) {
						(*bp)[i] += transition[i][j] * emission[j][*op] * (*bp_)[j];
					}
				}
				--bp; --bp_; --op;
			}
			ap = alpha.ptr(); bp = beta.ptr(1); op = observation.ptr(1);
			auto xp = xi.ptr(); 
			double sum = 0;
			sforin(t, 1, time) {
				sum = 0;
				sforin(i, 0, stnum) {
					sforin(j, 0, stnum) {
						(*xp)[i][j] = (*ap)[i] * transition[i][j] * emission[j][*op] * (*bp)[j];
						sum += (*xp)[i][j];
					}
				}
				(*xp) /= sum;
				++ap; ++bp; ++op; ++xp;
			}
			(*xp).reset(0.0);
			sforeach2(gamma, xi) {
				sforin(i, 0, stnum) {
					E1_[i] = 0;
					sforin(j, 0, stnum) E1_[i] += E2_[i][j];
				}
			}
		}
		inline void bwmStep(const sveci& observation, svecd& init, svdvec& alpha, svdvec& beta, svdvec& gamma, smdvec& xi, smatd& emission, smatd& transition) {
			auto stnum = emission.row;
			auto obsnum = emission.col;
			init = gamma[0];
			auto gsum = sstat::sum(gamma);
			auto xsum = sstat::sum(xi);
			sforin(i, 0, stnum) {
				sforin(j, 0, stnum) {
					transition[i][j] = xsum[i][j] / gsum[i];
				}
			}
			emission.reset(0.0);
			sforeach2(gamma, observation) {
				sforin(i, 0, stnum) {
					sforin(j, 0, obsnum) {
						if (E2_ == j) emission[i][j] += E1_[i];
					}
				}
			}
			sforin(i, 0, stnum) {
				sforin(j, 0, obsnum) emission[i][j] /= gsum[i];
			}
			
		}
		inline void BaumWelch(const sveci& observation, svecd& init, smatd& emission, smatd& transition, sint iter = 100) {
			sint time = observation.size();
			svdvec alpha(time, svecd(emission.row)), beta(time, svecd(emission.row)), gamma(time, svecd(emission.row));
			smdvec xi(time, smatd(transition.row, transition.col));
			sforin(i, 0, iter) {
				bweStep(observation, init, alpha, beta, gamma, xi, emission, transition);
				bwmStep(observation, init, alpha, beta, gamma, xi, emission, transition);
				



			}
		}
		inline void searchPath(sint &output, svecd &prev, svecd &current, sveci &track, svecd &tmp, smatd& emission, smatd& transition, bool logp) {
			sforin(s, 0, emission.row) {
				tmp.reset(0);
				sforin(p, 0, emission.row) {
					if (logp) tmp[p] = prev[p] + log(transition[p][s]) + log(emission[s][output]);
					else tmp[p] = prev[p] * transition[p][s] * emission[s][output];
				}
				track[s] = sstat::argmax(tmp);
				current[s] = tmp[track[s]];
			}
		}
		inline void traceBack(sveci& state, svecd& prob, svivec &track, size_t off = 0) {
			auto idx = sstat::argmax(prob);
			auto st = state.ptr(off + track.size());
			auto it = track.end() - 1;
			*st = idx; --st;
			while (track.begin() <= it) {
				idx = E_[idx];
				*st = idx;
				PREV_; --st;
			}
		}
		inline void initViterbiProb(sint& output, svecd& prob, smatd& emission) {
			sforin(i, 0, prob.size()) {
				prob[i] = emission[i][output];
			}
		}
		extern void Viterbi(sveci& state, sveci& observation, smatd& emission, smatd& transition, bool logp, bool off) {
			state.resize(observation.size());
			svecd tmp(emission.row);
			svdvec prob;
			svivec track;
			if (off) {
				prob.resize(observation.size());
				track.resize(observation.size());
				auto pit = prob.begin() + 1;
				auto tit = track.begin();
				initViterbiProb(observation[0], prob[0], emission);
				sforeach(observation) {
					searchPath(E_, *(pit - 1), *pit, *tit, tmp, emission, transition, logp);
				}
			}
			else {
				prob.resize(2);
				track.resize(1);
				auto pit = prob.begin() + 1;
				auto tit = track.begin();
				initViterbiProb(observation[0], prob[0], emission);
				auto it = observation.begin(), end = observation.end();
				while (it < end) {
					searchPath(E_, *(pit - 1), *pit, *tit, tmp, emission, transition, logp);
					





					NEXT_;
				}
			}
			traceBack(state, prob.last(), track);
		}

		class HMMAnalysis {
		public:
			sint iter;
			sveci state, *observation;
			smatd emission, transition;

		public:
			HMMAnalysis(sveci& data, sint st, sint obs) {
				observation = &data;
				emission.resize(st, obs);
				transition.resize(st, st);
			}
			~HMMAnalysis() {}

			void setProbs(const smatd& e, const smatd& t) {
				emission = e;
				transition = t;
			}
			void train(svecd& init, sint i = 100) {
				iter = 100;
				ssci::BaumWelch(*observation, init, emission, transition, i);
			}
			void predict(sveci &data, bool logp = true, bool off = true) {
				ssci::Viterbi(state, *observation, emission, transition, logp, off);
			}
		};
	}
}
#endif