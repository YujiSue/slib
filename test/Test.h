#include "sconfig.h"

#define MATH_TEST

#define MEM_TEST
#define RANGE_TEST
#define AREA_TEST
#define PTR_TEST
#define ARRAY_TEST
#define LIST_TEST
#define MAP_TEST
#define CHAR_TEST
#define STRING_TEST
#define TIME_TEST

#define SOBJECT_TEST
/*
#define SNUMBER_TEST
#define SSTRING_TEST
#define SDATE_TEST
#define SDATA_TEST
#define SARRAY_TEST
#define SDICT_TEST
#define TEXT_TEST
#define FUNC_TEST
#define IO_TEST

*/
#define TABLE_TEST
#define DB_TEST

#define THREAD_TEST
#define PROCESS_TEST

//#define APP_TEST 1
//#define CV_TEST


namespace test {
	extern void CalcTest();
	extern void FracTest();
	extern void ComplexTest();
	extern void LinearAlgebraTest();
	extern void GeometryTest();
	extern void StatisticTest();

}
namespace test {
	extern void MemoryTest(); 
	extern void RangeTest();
	extern void PtrTest();
	extern void ArrayTest();
	extern void ListTest();
	extern void MapTest();
	extern void CharTest();
	extern void StringTest();

}
namespace test {
	extern void SObjTest();
	extern void SNumberTest();
	extern void SStringTest();
	extern void SDateTest();
	extern void SDataTest();
	extern void SArrayTest();
	extern void SDictTest();

	extern void SFuncTest();

	extern void STableTest();
	

}
namespace test {
	extern void SDBTest();
	extern void ThreadTest();
}

#ifdef NODE_TEST
class TestNode : public Node<TestNode> {
public:
	sushort type;
	String label;

	TestNode(sushort t, const char* l) : Node<TestNode>() {
		type = t;
		if (l) label = l;
		else label = "root";
	}
	~TestNode() {}

};
#endif

/*



	String res;
	SSystem::exec("python -V", res);

	std::cout << (res.equal(REG(/python\\s[0-9]+\\.[0-9]+\\.[0-9]\\s/i)) ? "match" : "no") << std::endl;

	SWork worker(4);
	sforin(i, 0, 10) {
		worker.addTask(waitprint);
	}
	worker.complete();


	//std::vector<std::mutex> mtxvec(8);
	UArray<std::mutex> mtxarray(8);
	//Array<std::thread> threads(8);
	{
		std::unique_lock<std::mutex>(mtxarray[3]);
	}


	SFunction<int(float)> func__1 = [](float f) { return (int)f; };
	std::cout << func__1(1.2f) << std::endl;

	sdict dct1 = { kv("id", 1), kv("name", u8"名前"), kv("value", snull) };
	auto it = dct1.begin();
	std::cout << *it << std::endl;
	auto it2 = dct1.end();
	std::cout << *it2 << std::endl;
	bool b = it < it2;
	sforeach(dct1) {
		std::cout << E_.key() << ":" << E_.value() << std::endl;
	}
	dct1.clear();
	sforeach(dct1) {
		std::cout << E_.key() << ":" << E_.value() << std::endl;
	}



	pl1(snull);
	std::cout << sio::SFile(DEFAULT_PREF_PATH).path() << std::endl;


	typedef void (*plug1)(void);
	using plug2 = int(*)(int);
	//int (*plug2)(int);

	plug1 pl_1;
	plug2 pl_2;

#ifdef _WIN32
	HMODULE handle;
	String path(u8"DylibTest.dll");
	handle = LoadLibrary(path.unicode().c_str());
#else
	void* handle;
	handle = dlopen("plugin/DylibTest.so", RTLD_LAZY);
#endif

	if (!handle) {
		printf("cant open DylibTest.dll\n");
		exit(1);
	}

#ifdef _WIN32
	pl_1 = (plug1)GetProcAddress(handle, "testcall");
	pl_2 = (plug2)GetProcAddress(handle, "testcall2");
#else
	* (void**)(&func1) = dlsym(handle, "testcall");
	*(void**)(&func2) = dlsym(handle, "testcall2");
#endif

	pl_1();
	int retval = pl_2(3);
	printf("func2 returned %d\n", retval);

#ifdef _WIN32
	FreeLibrary(handle);
#else
	dlclose(handle);
#endif



extern inline sobj test_plugin(sobj obj) {
	std::cout << "Test" << std::endl;
	return snull;
}

using SPlugIn = sobj(*)(sobj);
SPlugIn pl1 = &test_plugin;
SFunction<sobj(sobj)> func_1(pl1);
inline void waitprint() {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "test thread" << std::endl;
}

snet::SNetWork nw;
	nw.http("http://160.24.63.91/login.php", false, u8"C:\\Users\\yujis\\Downloads\\cookie.txt", "username=elegans&password=worms");
	std::cout << (const char *)nw.data.ptr() << std::endl;
	nw.http("http://160.24.63.91/mutant_edit.php?MUTANT_ID=14045", false, u8"C:\\Users\\yujis\\Downloads\\cookie.txt");
	std::cout << (const char*)nw.data.ptr() << std::endl;



	SZip sz(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\Book1.xlsx");
	SString context;
	context.load(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\[Content_Types].xml");
	std::cout << "CRC:" << SChecker::crc32((uint8_t*)context.ptr(), context.length()) << std::endl;


	SFunction<void(const char *)> func1([](const char* message) { std::cout << String(message) << std::endl; });
	func1("abc");
	std::cout << func1.toString() << std::endl;
	SFunction<int(int,int)> func2 = [](int i1, int i2) { return i1 + i2; };
	std::cout << func2.exec(1, 2) << std::endl;
	SFunction<void()> func3 = []() {};


	STable table1("table",
		{
			SColumn(SColumn::INTEGER_COLUMN, "ID"),
			SColumn(SColumn::STRING_COLUMN, "NAME"),
			SColumn(SColumn::REAL_COLUMN, "VALUE")
		});
	//table1.addRow({ 0, "", 0.5 });

	//table2.load("");



	//SImage image;
	smedia::SImage img;
	img.load(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Research\\Article\\NGS\Fig\\Figure_1.png");
	img.save(u8"C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\test.tiff");

	//SCanvas canvas;


	sdb::SDataBase db;
	db.open(HOME_PATH + PATH_SEPARATOR + "test.db");
	//db.createTable();
	std::cout << "count:" << db.tableCount() << std::endl;
	std::cout << db.tables() << std::endl;
	//db.open("C:\\Users\\yujis\\OneDrive\\ドキュメント\\Develop\\Hayamin\\db\\hayami.db");
	//std::cout << db.tables() << std::endl;


*/