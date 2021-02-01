#ifndef SSCI_SCHART_H
#define SSCI_SCHART_H

#include "sobj.h"
#include "sapp/sglapp.h"

//#define DEFAULT_RENDERER &displayGL

namespace slib {
	namespace ssci {

		constexpr sushort SCATTER_PLOT = 0x01;
		constexpr sushort BAR_GRAPH = 0x02;
		constexpr sushort PI_GRAPH = 0x03;
		constexpr sushort LINE_GRAPH = 0x04;

		constexpr sushort TREE_DIAGRAM = 0x10;



		typedef void(*RENDERER)(smedia::SCanvas& cnvs);


		class SChart {
			
		protected:
			RENDERER _render;
			suinteger _type;
			String _title;
			smedia::SCanvas _canvas;
			SDictionary _attribute;

		public:
			SChart();
			SChart(sushort t);
			~SChart();

			void load(const char* path, SDictionary &dic);
			
			template<class... Args>
			void load(const char* path, Args... args) {
				SDictionary pref;
				//(pref, args...);
			}

			void save(const char* p);
			void setRenderer(const char *name);
			void setData();
			void display();

		};
	}
}



#endif