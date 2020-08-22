#include "Test.h"
#include "sobj.h"

using namespace slib;
void test::SXmlTest() {
	SArray array = {
		1, 2.71828, "abc", u8"あいう", new SDate(slib::sstyle::YMD), V({ 10, "xyz" })
	};
	SDictionary dict = {
		kv("int", 100), kv("real", 1.732), kv("str", u8"ひふみ")
	};
	try {
		array.save(sio::currentPath() + PATH_SEPARATOR + u8"配列.plist");
		dict.save(sio::currentPath() + PATH_SEPARATOR + u8"辞書.plist");
	}
	catch (SIOException ie) { ie.print(); }
}

void test::SDBTest() {
	try {
		std::cout << sql::orderQue({
			kv("ID", ASC), kv("START", DESC)
			}) << std::endl;
		std::cout << sql::limit(10, 5) << std::endl;
		std::cout << sql::listQue({
			1, 3, 5, 7, 9
			}) << std::endl;
		std::cout << sql::caseQue({
			kv("if", V({ kv("col", "SCORE"), kv("op", ">"), kv("val", 10), kv("res", "CLEAR") })),
			kv("else", "OUT"),
			kv("as", "JUDGE")
			}) << std::endl;
		std::cout << sql::condQue({
			kv("and", V({
				V({ kv("col", "LENGTH"), kv("op", "<"), kv("val", 5.0) }),
				V({ kv("col", "NAME"), kv("op", "like"), kv("val", "A"), kv("match", 3) })
			}))
			}) << std::endl;
		std::cout << sql::selectQue("Table", {
			kv("cols", V({ "COL1", "COL2" })),
			kv("order", V({ kv("ID", ASC), kv("START", DESC) })),
			kv("limit", V({ kv("lim", 1), kv("off", 5) })),
			kv("condition", V({ kv("and", V({
				V({ kv("col", "LENGTH"), kv("op", "<"), kv("val", 5.0) }),
				V({ kv("col", "NAME"), kv("op", "like"), kv("val", "A"), kv("match", 3) })
			})) }))
			}) << std::endl;

		sio::SFile f(sio::currentPath() + PATH_SEPARATOR + "test.db");
		if (f.exist()) f.remove();
		SDataBase db1(f);
		db1.createTable("Test1",
			{
				scolumn(INTEGER_COLUMN | sql::KEY_COLUMN, "ID"),
				scolumn(STRING_COLUMN, "NAME"),
				scolumn(STRING_COLUMN, "VALUE")
			});
		db1.createTable({
			kv("name", "Test2"),
			kv("columns", V({
				V({
					kv("type", INTEGER_COLUMN | sql::KEY_COLUMN),
					kv("name", "TID")
				}),
				V({
					kv("type", STRING_COLUMN),
					kv("name", "PERSON")
				}),
				V({
					kv("type", ARRAY_COLUMN),
					kv("name", "ADDRESS")
				}),
				})),
			kv("rows",
			V({
					V({ 1, u8"山田", V({ u8"東京", u8"新宿" })}),
					V({ 2, u8"鈴木", V({ u8"大阪", u8"中央" })})
				}))
			});

		std::cout << db1.tableCount() << " " << db1.tables() << std::endl;
		db1["Test1"].addColumn(STRING_COLUMN, "COL");
		std::cout << db1["Test1"].columnInfo() << std::endl;
		db1["Test1"].addRecord({ snull, "first", u8"あ", snull });
		db1["Test1"].addRecord({ kv("NAME", "second"), kv("VALUE", u8"い") });
		db1["Test1"].getRecordPrepare();
		std::cout << db1.getRows() << std::endl;
		db1.commit();
		db1["Test1"].removeColumn("NAME");
		std::cout << db1["Test1"].columns() << std::endl;
		std::cout << db1["Test2"].getRecords() << std::endl;
		db1["Test2"].removeRecordAt(1, "TID");
		std::cout << db1["Test2"].getRecords() << std::endl;
		db1.close();

		SDataBase db2(f);
		db2["Test2"].addRecord({ snull, u8"小鳥遊", snull });
		db2["Test2"].addRecord({ snull, u8"一", snull });
		db2["Test2"].addRecord({ snull, u8"四月一日", snull });
		db2["Test2"].addRecord({ snull, u8"五月七日", snull });
		std::cout << db2["Test2"].getRecords({
			kv("cols", V({"TID", "PERSON"})),
			kv("condition", V({
				kv("col", "TID"), kv("op", ">"), kv("val", 2)
				}))
			}) << std::endl;
	}
	catch (SDBException ex) { ex.print(); }
}

#ifdef THREAD_TEST
inline void thread_test(int i, SThread *thread) {
	std::this_thread::sleep_for(std::chrono::seconds(i));
	std::cout << "Thread exec." << std::endl;
	thread->setState(SThread::COMPLETED);
}
inline void thread_pool_test(int i) {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << "Task No. " << i << " exec." << std::endl;

}
void test::ThreadTest() {
	SThread th1;
	th1.start(thread_test, 5, &th1);
	while (th1.state() == SThread::WORKING) {
		std::cout << "Main wait." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	th1.complete();
	SWork work(8);
	sforin(i, 0, 10) {
		work.addTask(thread_pool_test, i + 1);
	}
	work.complete();


}

#endif