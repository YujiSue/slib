#ifndef SSCI_ML_H
#define SSCI_ML_H

namespace slib {
	namespace ssci {
		typedef enum {
			TRAINING = 1,
			PREDCITION = 2,
		} ML_MODE;

		extern inline double sigmoid(double n) { return 0.5 * (tanh(0.5 * n) + 1.0); }
		extern inline double splus(double n) { return log(1.0 + exp(n)); }
		extern inline double relu(double n) { return 0.0 < n ? n : 0.0; }


		extern inline void em() {

		}

		class EMAnalysis {

		};

	}
}
#endif