#ifndef SSCI_PCA_H
#define SSCI_PCA_H

#include "smath/stat.h"

namespace slib {
	namespace ssci {
		class SDataFrame;

		extern /*SSCI_DLL*/ void PCAAnalysis(SDataFrame &df, const sveci &cols) {
			


		}

		template<typename T, class M>
		extern void pca(svec<svec<T, M>>& vec, svdvec& evec, svecd& eval) {
			auto mat = sstat::covmat(vec);
			sla::eigen(mat, evec, eval);
		}
	}
}
#endif