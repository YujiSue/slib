#include "sapp/scvapp.h"

using namespace slib;
using namespace slib::sapp;

SCVApp::SCVApp() : SApp() { _launch = true; }
SCVApp::SCVApp(const char* path) : SApp(path) {}
SCVApp::SCVApp(SDictionary&& prof) : SApp(std::forward<SDictionary &&>(prof)) {}
SCVApp::~SCVApp() {}

void SCVApp::init() {
	_mainframe = cv::Mat(600, 800, CV_8UC4);
	_mainframe = cv::Scalar(200, 200, 200);
	cv::namedWindow("Main");
	cv::imshow("Main", _mainframe);
	
	//_scmanager.setShortCut(27, &quit);
}
int SCVApp::run() {
	try {
		init();
		while (_launch) {
			_key = cv::waitKeyEx(20);

			switch (_key)
			{

			case 27:
				quit();
				break;
			default:
				break;
			}
		}
		return 0;
	}
	catch (cv::Exception ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}
	catch (SAppException ae) {
		ae.print();
		return 1;
	}
}

void SCVApp::quit() {
	_launch = false;
}