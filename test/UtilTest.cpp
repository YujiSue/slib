#include "Test.h"
#include "sobj.h"

using namespace slib;

void test::SDBTest() {
	sio::SFile f(sio::currentPath() + PATH_SEPARATOR + "test.db");
	if (f.exist()) f.remove();
	SDataBase db1(sio::currentPath() + PATH_SEPARATOR + "test.db");
	db1.createTable("Test1",
		{
			scolumn(SColumn::INTEGER_COLUMN | sql::KEY_COLUMN, "ID"),
			scolumn(SColumn::TEXT_COLUMN, "NAME"),
			scolumn(SColumn::TEXT_COLUMN, "VALUE")
		});
	db1.createTable({
		kv("name", "Test2"),
		kv("name", "Test2"),
		kv("name", "Test2")
		});
	

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