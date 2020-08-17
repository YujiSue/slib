#ifndef SSCI_PCA_H
#define SSCI_PCA_H

#include "smath/stat.h"

namespace slib {
	namespace ssci {
		class SDataFrame;

		extern /*SSCI_DLL*/ void PCAAnalysis(SDataFrame &df, const sveci &cols) {
			svdvec


		}


		extern void pca(SVector<SVector<T, M>>& vec, svdvec& evec, svecd& eval) {
			auto mat = ssstat::covmat(vec);
			SLA::eigen(mat, evec, eval);
		}
	}
}
#endif