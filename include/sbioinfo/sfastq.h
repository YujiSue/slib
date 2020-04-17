#ifndef SBIO_SFASTQ_H
#define SBIO_SFASTQ_H

#include "sbioinfo/sbioinfoutil.h"
#include "sbioinfo/sbioseq.h"

namespace slib {
	namespace sbio {
		class SFastq : public SBSeqList {
		public:
			SFastq();
			~SFastq();

			void importFq(const char* p);
			void exportFq(const char* p);
		};
	}
}

#endif
