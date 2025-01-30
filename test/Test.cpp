#include "Test.h"

using namespace slib;
int main() {

#ifdef BASIC_TEST
		test::RangeTest();
		test::AreaTest();
		test::ZoneTest();

		test::CharTest();
		test::StringTest();
		test::TimeTest();

		test::ExceptionTest();
#endif
#if defined(BASIC_TEST) || defined(MEM_TEST)
		test::MemoryTest();
#endif
#if defined(BASIC_TEST) || defined(PTR_TEST)
		test::PtrTest();
#endif
#if defined(BASIC_TEST) || defined(ARRAY_TEST)
		test::ArrayTest();
#endif
#if defined(BASIC_TEST) || defined(REGION_TEST)
		test::RegionTest();
#endif
#if defined(BASIC_TEST) || defined(LIST_TEST)
		test::ListTest();
#endif
#if defined(BASIC_TEST) || defined(MAP_TEST)
		test::MapTest();
#endif
#if defined(BASIC_TEST) || defined(SET_TEST)
		test::SetTest();
#endif
#if defined(BASIC_TEST) || defined(NODE_TEST)
		test::NodeTest();
#endif

#ifdef MATH_TEST
		test::CalcTest();
		test::FracTest();
		test::ComplexTest();
		test::LATest();
		test::GeomTest();
		test::StatTest();
#endif

#ifdef SOBJ_TEST
		test::SNumberTest();
		test::SStringTest();
		test::SDateTest();
		test::SDataTest();
		test::SArrayTest();
		test::SDictTest();
		test::SFuncTest();
		test::SVoidTest();
		test::STableTest();
#endif

#ifdef SUTIL_TEST

		test::SDBTest();

#endif
		return 0;

#ifdef APP_TEST
	test::SCUIAppTest();
	//test::SPlugInAppTest();

	{
		//test::SCodeTest();
	}
#endif


#ifdef SBIOINFO_TEST
		test::SBAnnotTest();
		test::SBSeqIOTest();
		//test::SBSeqTest();
#endif

#ifdef oldtest


#include "sapp/scuiapp.h"

int main(int argc, const char** argv) {
	TestApp app;
	return app.run(argc, argv);
}

#elif defined(CV_TEST)
#include "opencv/cv.h"
#include "opencv2/highgui.hpp"

int main(int argc, const char** argv) {
	sql::selectQue(_table, info);


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
#enedif

class StringNode : public SNode<StringNode> {
private:
	String str;
public:
	StringNode() {}
	StringNode(const char* s) : str(s) {}
	~StringNode() {}

};

void print(int i) {
	std::cout << i << std::endl;
}
sobj print2(int i, int j) {
	return String(i) + String(j);
}
sobj print3(sobj args) {
	//auto cls = args.toClass<Set<int, int>>();
	std::cout << args[0] << "," << args[1] << std::endl;
	return snull;
}
sobj func1(int i, double d, String s) {
	return i * d * s.integer();
}
sobj func2(sobj obj) {
	std::cout << obj.toString() << std::endl;
	return snull;
}
template<class... Args>
struct PlugInTest {
	typedef sobj(*PLUGIN)(Args...);
	PLUGIN plugin;
	void setFunc(PLUGIN f) { plugin = f; }

};
struct PlugInTest2 {
	typedef sobj(*PLUGIN)(sobj);
	PLUGIN plugin;
	void setFunc(PLUGIN f) { plugin = f; }
	template<class... Args>
	sobj exec(Args... args) { return (*plugin)(Set<Args...>(args...)); }
};
int main_()
{


	/*
	String s1 = "ATCGAGCAGTGCGATGTTCAGGT";
	String s2 = "AGCATTGTACGCAGTGCAGACAT";
	String s3 = "ATCCSGCAGTGGGATGTTATGGT";
	ubytearray s1_, s2_, s3_;
	sbio::sseq::dencode(1, s1, s1_);
	sbio::sseq::dencode(1, s2, s2_);
	sbio::sseq::dencode(1, s3, s3_);

	sbio::salign_param par(sbio::DNA_SEQ);
	par.align_length = 50;

	sbio::SAlignment aligner(&par);
	aligner.align(s1_.ptr(), s1_.size(), s2_.ptr(), s2_.size(), false);
	std::cout << aligner.scores.first() << std::endl;
	std::cout << aligner.cigars.toString() << std::endl;

	sbio::salign al(sbio::sbpos(0, 0, 20), srange(0, 20));
	al.cigars = aligner.cigars;
	auto cs = al.consensus(s1, s2);
	std::cout << cs << std::endl;

	ubytearray cs_;
	sbio::sseq::dencode(1, cs, cs_);
	aligner.align(cs_.ptr(), cs_.size(), s3_.ptr(), s3_.size(), false);
	std::cout << aligner.scores.first() << std::endl;
	std::cout << aligner.cigars.toString() << std::endl;

	al = sbio::salign(sbio::sbpos(0, 0, cs.length()), srange(0, 20));
	al.cigars = aligner.cigars;
	std::cout << al.consensus(cs, s3) << std::endl;


	std::function<double(subyte*, subyte*, int)> similarity = [&aligner](subyte* s1_, subyte* s2_, int len) {
		aligner.align(s1_, len, s2_, len);
		aligner.scores.first();

		return (double)0;
	};
	*/
	//ssstat::kmeans(100, )

	/*
	svdvec data(10), centroid;
	sveci group;
	voidarray clusters;
	SRandom rand1, rand2;
	sforin(i, 0, data.size()) {
		auto m = rand1.iruni(0, 1);
		data[i] = { rand2.rnorm(m, 0.25), rand2.rnorm(m, 0.25) };
	}
	ssci::hcluster(data, clusters);

	sforeach(data) {
		std::cout << INDEX_(data) << " " << E_[0] << "," << E_[1] << std::endl;
	}

	printCluster(0, (ssci::scluster *)clusters[0]);
	*/
	/*
	ssci::kmeans(2, data, group, centroid, 300, "kmeans++");
	sforin(i, 0, 100) {
		std::cout << data[i][0] << "," << data[i][1] << "," << group[i] << std::endl;
	}
	*/
	/*
	StringNode node("abc");
	node.addChild(new StringNode("123"));
	node[0]->addChild(StringNode("DoReMi"));
	std::cout << node[0]->layer() << std::endl;
	std::cout << node[0]->children()[0]->layer() << std::endl;
	*/
	/*
	sregion region;
	sio::SFile file("F:\\target.bed");
	String row;
	while (!file.eof()) {
		file.readLine(row);
		auto values = row.split(TAB);
		if (row.empty() || row[0] == '#' || values.size() < 3) continue;
		region.add(srange(values[1], values[2]));
	}
	std::cout << region[0].begin << ".." << region[0].end << std::endl;
	*/
	/*
	sio::SFile file = sio::SFile("F:\\log.txt", sio::APPEND);
	std::cout << (file.isOpened() ? "open" : "close") << std::endl;

	file << "Test" << NEW_LINE;
	file.flush();
	*/
	/*
	SNetWork net;
	auto info = STANDARD_HTTPS;
	info["url"] = "https://10.151.64.21/login.php";
	info["post"] = "username=elegans&password=worms";
	info["cookie"] = "C:\\Users\\yujis\\cookie.txt";
	info["ignore-ssl"] = true;
	try {
		net.connect(info);
	}
	catch (SException ex) { ex.print(); }
	auto info2 = STANDARD_HTTPS;
	info2["url"] = "https://10.151.64.21/primer.php";
	info2["cookie"] = "C:\\Users\\yujis\\cookie.txt";
	info2["ignore-ssl"] = true;
	try {
		net.connect(info2);
		String str;
		net.data.trans(str);
		std::cout << str << std::endl;
	}
	catch (SException ex) { ex.print(); }
	*/

#ifdef MATH_TEST
	test::CalcTest();
	test::FracTest();
	test::ComplexTest();
	test::LinearAlgebraTest();
	test::GeometryTest();
	test::StatisticTest();

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
	std::cout << proc.message() << std::endl;
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
	std::cout << E_["NAME"] << TAB << E_["ATTRIBUTE"] << std::endl;
}
*/
/*
sdb::SDataBase wdb(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Research\\worm.db");
//std::cout << hdb.tables() << std::endl;
auto records = wdb["CHROMOSOME"].getRecords();
sforeach(records) {
	std::cout << E_["NAME"] << TAB << E_["LENGTH"] << std::endl;
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
/*
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

	*/