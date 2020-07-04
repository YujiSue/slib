#ifndef SSCI_SCHART_H
#define SSCI_SCHART_H

#include "sobj.h"

namespace slib {
	namespace ssci {

		constexpr sushort BAR_GRAPH = 0x0001;
		constexpr sushort PI_GRAPH = 0x0002;
		constexpr sushort LINE_GRAPH = 0x0003;
		constexpr sushort PLOT_GRAPH = 0x0004;

		class SChart {
		protected:
			sushort _type;
			String _title;
			SDictionary _attribute;

		public:
			SChart();
			SChart(sushort t);
			~SChart();

			void load(const char* p);
			void save(const char* p);

			void setData();
			void display();

		};
	}
}



#endif