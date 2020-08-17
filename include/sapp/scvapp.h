#ifndef SAPP_SCVAPP_H
#define SAPP_SCVAPP_H

#include "sobj.h"
#include "sapp/sapp.h"
#include "smedia/scv.h"
//#include "cvui.h"

namespace slib {
    namespace sapp {
		class SShortCutManager {
		protected:
			//Map<suint, sfunc<void()>> _actions;

		public:
			SShortCutManager() {}
			~SShortCutManager() {}
			/*
			void setShortCut(suint key, sfunc<void()> func) {
				_actions.set(key, func);
			}
			sfunc<void()>& operator[](suint key) {
				return _actions[key];
			}
			*/
		};

		/*
		class SCVWindow {
		protected:
			cv::Mat _frame;
			cv::String _title;

		public:
			SCVWindow();
			SCVWindow(SDictionary&& prof);
			~SCVWindow();


		};
		using cvwindow = Pointer<SCVWindow>;
		class SCVWindowManager {
		protected:
			List<cvwindow> _windows;

		public:
			SCVWindowManager();
			~SCVWindowManager();

			cvwindow top();
			
		};
		*/


        class SCVApp : public SApp {
		protected:
			sint _key;
			bool _launch;
			String _title;
			cv::Mat _mainframe;
			SShortCutManager _scmanager;

        public:
			SCVApp();
			SCVApp(const char *path);
			SCVApp(SDictionary &&prof);
            ~SCVApp();
            
            int run();
			virtual void init();
			
			//void showAbout();
			//void showPref();
			void quit();

        };
    }
}

#endif
