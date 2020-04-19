#include "Test.h"
#include "sutil/sutil.h"

using namespace slib;

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