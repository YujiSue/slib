#include "sutil/sthread.h"

slib::SLock::SLock() { _lock.exchange(false); }
slib::SLock::SLock(const slib::SLock& l) {}
slib::SLock::~SLock() {}
slib::SLock& slib::SLock::operator=(const slib::SLock& l) { return *this; }
void slib::SLock::lock() { while (_lock.exchange(true, std::memory_order_acquire) == true) {} }
void slib::SLock::unlock() { _lock.store(false, std::memory_order_release); }
slib::SAutoLock::SAutoLock(slib::SLock& l) { _lock = &l; _lock->lock(); }
slib::SAutoLock::~SAutoLock() { _lock->unlock(); }
slib::SThread::SThread() : _id(0) , _state(NO_TASK) {}
slib::SThread::SThread(suinteger i) : _id(i), _state(NO_TASK) {}
slib::SThread::SThread(const SThread& t) : SThread::SThread() {}
slib::SThread::~SThread() {}
slib::SThread::STATE slib::SThread::state() const { return _state; }
void slib::SThread::setState(slib::SThread::STATE st) { _state = st; }
void slib::SThread::complete() {
	_thread.join();
	_state = COMPLETED;
}
slib::STask::STask() {}
slib::STask::~STask() {}
slib::STask& slib::STask::operator=(const slib::STask& t) {
	_func = t._func; return *this;
}
void slib::STask::operator()() { _func(); }
inline void _work(bool *run, slib::SThread *thread, std::condition_variable *cv, std::queue<slib::STask> *tasks, std::mutex *mtx) {
	while (*run) {
		slib::STask task;
		{
			std::unique_lock<std::mutex> lock(*mtx);
			if (tasks->empty()) {
				thread->setState(slib::SThread::WAITING);
				cv->wait(lock, [run, tasks]() {
					return !(*run) || !(tasks->empty());
					});
				if (!(*run) || tasks->empty()) break;
			}
			thread->setState(slib::SThread::WORKING);
			if (tasks->size()) {
				task = std::move(tasks->front());
				tasks->pop();
			}
		}
		task();
	}
	if (tasks->empty()) thread->setState(slib::SThread::COMPLETED);
	else thread->setState(slib::SThread::CANCELED);
}
slib::SWork::SWork() : _run(false), _count(0) {}
slib::SWork::SWork(size_t size) : slib::SWork() { setSize(size); }
slib::SWork::~SWork() {
	abort();
	_cv.notify_all();
	sfor(_threads) $_.complete();
}
void slib::SWork::_init() {
	_run = true;
	sfor(_threads) $_.start(_work, &_run, &$_, &_cv, &_tasks, &_mtx);
}
void slib::SWork::setSize(size_t size) {
	if (_count && isWorking()) {
		abort();
		_cv.notify_all();
		sfor(_threads) $_.complete();
	}
	_count = size;
	Array<SThread> tmp(_count);
	tmp.swap(_threads);
	_init();
}
bool slib::SWork::isWorking() {
	if (_run && _tasks.size()) return true;
	sfor(_threads) {
		if ($_.state() == SThread::WORKING) return true;
	}
	return false;
}
void slib::SWork::complete() {
	while (isWorking()) std::this_thread::sleep_for(std::chrono::nanoseconds(1));
}
void slib::SWork::abort() {
	_run = false;
}