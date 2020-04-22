#include "Test.h"
#include "sobj.h"

using namespace slib;

#ifdef APP_TEST
#if APP_TEST==1
#include "sapp/scuiapp.h"

class TestApp : public sapp::SCuiApp {
public:
	TestApp() : SCuiApp({
			kv("app", V({
					kv("name", "Test"),
					kv("type", 0),
					kv("creator", "Souki"),
					kv("version", "1.0.0"),
					kv("develop", "2012/12/5"),
					kv("license", "MIT license.")
				})
			),
			kv("command", V({ kv("_exec", V({ kv("require", V({ "alpha", "_args" })) })) })),
			kv("option", 
				V({
					kv("alpha", V({ kv("short", "a"), kv("type", "bool"), kv("description", "Set alpha mode.") })),
					kv("beta", V({ kv("short", "b"), kv("caption", "text"), kv("description", "Set param string.") })),
					kv("gamma", V({ kv("short", "c"), kv("type", "num"), kv("caption", "integer"), kv("description", "Set param value.") })),
					kv("_args", V({ kv("description", "argments...") }))
				})
			)
		}
	) {}
	~TestApp() {}
	int exec() {
		std::cout << "Exec app." << std::endl;
		return 0;
	}

};
int main(int argc, const char** argv) {
	TestApp app;
	return app.run(argc, argv);
}

#endif
#elif defined(CV_TEST)
#include "opencv/cv.h"
#include "opencv2/highgui.hpp"

int main(int argc, const char** argv) {
	bool rec = false;
	cv::VideoCapture cap(0);
	cv::Mat frame;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.read(frame);
	auto fourcc = cv::VideoWriter::fourcc('W', 'M', 'V', '1');
	auto writer = cv::VideoWriter("F:\\output.wmv", fourcc, 30.0, frame.size());
	if (!cap.isOpened()) return -1;
	cv::Mat disp(cv::Size(1, 1), CV_8UC1);
	while (cap.read(frame))
	{
		cv::imshow("win", disp);
		const int key = cv::waitKey(1);
		if (key == 'q') break;
		else if (key == 's') {
			cv::imwrite("F:\\img.png", frame);
		}
		else if (key == 'r') rec = !rec;
		if (rec) {
			writer << frame;
		}
	}
	cv::destroyAllWindows();
	return 0;
}
#else


//#include "sbio/sbioinfo.h"

int main()
{
#ifdef MATH_TEST
	test::CalcTest();

#endif
#ifdef MEM_TEST
	test::MemoryTest();
#endif
#ifdef RANGE_TEST
	test::RangeTest();
#endif
#ifdef PTR_TEST
	test::PtrTest();
#endif
#ifdef ARRAY_TEST
	test::ArrayTest();
#endif
#ifdef LIST_TEST
	test::ListTest();
#endif
#ifdef MAP_TEST
	test::MapTest();
#endif
#ifdef CHAR_TEST
	test::CharTest();
#endif
#ifdef STRING_TEST
	test::StringTest();
#endif


#ifdef SOBJECT_TEST
	test::SObjTest();
#endif

#ifdef THREAD_TEST
	//test::ThreadTest();
#endif

#ifdef PROCESS_TEST
	SSystem::setCurrent(u8"C:\\Users\\Yuji\\OneDrive\\ドキュメント\\Develop\\Pantheon");
	SProcess p0;
	p0.createChild("npm start");


	{
		SProcess p1;
		p1.createChild("C:\\Windows\\System32\\notepad.exe");
	}
	{
		SProcess p2;
		p2.createChild("C:\\Program Files\\Microsoft Games\\Solitaire\\Solitaire.exe", false);
	}

	SProcess proc;
	proc.createChild("Test2.exe");
	proc.send("o");
	std::cout << proc.message() <<std::endl;
	proc.send(u8"s:於伊邪那岐命、抜所御佩之十拳剣、斬其子迦具土神之頚", false);
	proc.send("q", false);
	proc.wait();

#endif




	return 0;
}

#endif



	




	
	/*
	TestNode node0(0, nullptr);
	node0.add(new TestNode(1, "node1"));
	node0.add(new TestNode(2, "node2"));
	node0[0]->add(new TestNode(11, "node11"));
	*/
	/*
	snum n1_ = 100;
	snum n2_ = 3.14;
	snum n3_ = sfrac(2, 5);
	snum n4_ = scomp(0.5, 0.5);

	std::cout << HOME_PATH << std::endl;
	std::cout << SSystem::charCode() << std::endl;
	sio::SFile file(u8"C:\\Users\\Yuji\\OneDrive\\ドキュメント\\Develop\\test.xlsx");
	SZip::expand(file);
	*/


	/*
	sdb::SDataBase hdb(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\Hayamin\\db\\hayami.db");
	//std::cout << hdb.tables() << std::endl;
	auto records = hdb["SAMPLE"].getRecords();
	sforeach(records) {
		std::cout << E_["NAME"] << String::TAB << E_["ATTRIBUTE"] << std::endl;
	}
	*/
	/*
	sdb::SDataBase wdb(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Research\\worm.db");
	//std::cout << hdb.tables() << std::endl;
	auto records = wdb["CHROMOSOME"].getRecords();
	sforeach(records) {
		std::cout << E_["NAME"] << String::TAB << E_["LENGTH"] << std::endl;
	}

	sio::SFile dir("\\\\160.24.63.106\\share\\iA_article");
	auto list = dir.fileList();

	char magic[4];


	sforeach(list) {
		std::cout << E_.path() << std::endl;
		if (E_.path().endWith(".bsm")) {
			E_.open();
			E_.readBytes(magic, 4);




			E_.close();
		}
	}


	*/
	/*
	sstruct<int, int, int> sst;
	std::cout << sst.f << "," << sst.s.f << ":" << sizeof(sst) << std::endl;
	
	for (size_t i = 0; i < 3; ++i) {
		std::cout << typeid(sarg_element<0, sstruct<int, int, int>>::type).name() << ",";

	}

	*/

#ifdef TEXT_TEST
	SText text1 = SColorText("red", u8"red/赤");
	std::cout << text1 << std::endl;
	SText text2 = SItalicText(u8"italic/斜体");
	std::cout << text2 << std::endl;
	SText text3(u8"色は匂へど散りぬるを我が世誰ぞ常ならむ");
	//text3.setAttribute(srange())
#endif
#ifdef IO_TEST
	std::cout << "home:" << SFile::home().path() << std::endl;
	std::cout << "current:" << SFile::current().path() << std::endl;
	SSystem::setCurrent(HOME_PATH);
	std::cout << "current:" << SFile::current().path() << std::endl;


	try {
		/*
		sio::SFile file1(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop");
		std::cout << "file1: exist?" << (file1.exist() ? "true" : "false") << " dir?" << (file1.isDir() ? "true" : "false") << std::endl;
		sio::SFile file2(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\test.text", sio::CREATE);
		file2 << "浅き夢みじ酔ひもせず";
		file2.close();
		
		sio::SFile file3(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\Test", sio::DIRECTORY);
		if (!file3.exist()) file3.make();
		file2.moveTo(file3);
		*/
	}
	catch (sio::SIOException ie) { 
		ie.print(); 
	}
#endif