#include "sutil/sthread.h"

using namespace slib;

SLock::SLock() { _lock.exchange(false); }
SLock::~SLock() {}
void SLock::lock() { while (_lock.exchange(true, std::memory_order_acquire) == true) {} }
void SLock::unlock() { _lock.store(false, std::memory_order_release); }
SAutoLock::SAutoLock(SLock& l) : _lock(&l) { _lock->lock(); }
SAutoLock::~SAutoLock() { _lock->unlock(); }

SThread::SThread() : _state(NO_TASK), _id(0) {}
SThread::SThread(suinteger i) : _state(NO_TASK), _id(i) {}
SThread::~SThread() {}
const SThread::STATE SThread::state() const { return _state; }
void SThread::setState(STATE st) { _state = st; }
void SThread::complete() { 
	_thread.join();
	_state = COMPLETED;
}


STask::STask() {}
STask::~STask() {}
STask& STask::operator=(const STask& t) {
	_func = t._func; return *this;
}
void STask::operator()() { _func(); }
#ifndef _MANAGED
inline void _work(bool *run, SThread *thread, std::condition_variable *cv, std::queue<STask> *tasks, std::mutex *mtx) {
	while (*run) {
		STask task;
		{
			std::unique_lock<std::mutex> lock(*mtx);
			if (tasks->empty()) {
				thread->setState(SThread::WAITING);
				cv->wait(lock, [run, tasks]() {
					return !(*run) || !(tasks->empty());
					});
				if (!(*run) || tasks->empty()) break;
			}
			thread->setState(SThread::WORKING);
			if (tasks->size()) {
				task = std::move(tasks->front());
				tasks->pop();
			}
		}
		task();
	}
	if (tasks->empty()) thread->setState(SThread::COMPLETED);
	else thread->setState(SThread::CANCELED);
}
#endif

SWork::SWork() : _run(false), _count(0) {}
SWork::SWork(size_t size) : SWork() { setSize(size); }
SWork::~SWork() {
	abort();
	_cv.notify_all();
	sforeach(_threads) E_.complete();
}
void SWork::_init() {
	_run = true;
	sforeach(_threads) E_.start(_work, &_run, &E_, &_cv, &_tasks, &_mtx);
}
void SWork::setSize(size_t size) {
	if (_count && isWorking()) {
		abort();
		_cv.notify_all();
		sforeach(_threads) E_.complete();
	}
	_count = size;
	UArray<SThread> tmp(_count);
	tmp.swap(_threads);
	_init();
}
bool SWork::isWorking() {
	if (_run && _tasks.size()) return true;
	sforeach(_threads) {
		if (E_.state() == SThread::WORKING) return true;
	}
	return false;
}
void SWork::complete() {
	while (isWorking()) std::this_thread::sleep_for(std::chrono::nanoseconds(1));
}
void SWork::abort() {
	_run = false;
}