#ifndef SLIB_STHREAD_H
#define SLIB_STHREAD_H

#ifdef _MANAGED
using namespace System::Threading;
#endif
#include "sbasic/sbasic.h"

namespace slib {
    class SLIB_DLL SLock {
#ifdef _MANAGED
	private:
		int _lock;

	public:
		SLock() { Interlocked::Exchange(_lock, 0); }
		~SLock() {}
		void lock() { Interlocked::CompareExchange(_lock, true, false); }
		void unlock() { Interlocked::CompareExchange(_lock, false, true); }

#else
	private:
		std::atomic<bool> _lock;

	public:
		SLock();
		~SLock();

		void lock();
		void unlock();
#endif 
    };

	class SLIB_DLL SAutoLock {
	private:
		SLock *_lock;
	public:
		SAutoLock(SLock& l);
		~SAutoLock();
	};

	class SLIB_DLL SThread {
	public:
		typedef enum {
			NO_TASK = 0,
			WORKING = 1,
			WAITING = 2,
			CANCELED = 3,
			COMPLETED = 4,
		} STATE;

	private:
		suinteger _id;
		STATE _state;
#ifdef _MANAGED

#else
		std::thread _thread;
#endif

	public:
		SThread();
		SThread(suinteger i);
		~SThread();

		template<class Func, class... Args>
		void start(Func f, Args... args) {
			_state = SThread::WORKING;
#ifdef _MANAGED

#else
			_thread = std::thread(f, args...);
#endif
		}
		const STATE state() const;
		void setState(STATE st);
		void complete();
	};

	class SLIB_DLL STask {
	
	private:
		std::function<void()> _func;

	public:
		STask();
		template<class Func, class... Args>
		STask(Func& func, Args... args) {
			_func = std::bind(func, args...);
		}
		~STask();
		
		STask& operator=(const STask& t);
		template<class Func, class... Args>
		void setTask(Func& func, Args... args) {
			_func = std::bind(func, args...);
		}
		void operator()();

	};
	class SLIB_DLL SWork {
	public:
		/*
		typedef enum {
			FIRST = 0,
			SECOND = 1,
			NORMAL = 2,
			LESSER = 3,
		} PRIORITY;
		*/
	private:
		bool _run;
		size_t _count;
		UArray<SThread> _threads;
		std::queue<STask> _tasks;

#ifdef _MANAGED

#else
		std::condition_variable _cv;
		std::mutex _mtx;
#endif

	private:
		void _init();

	public:
		SWork();
		SWork(size_t s);
		~SWork();

		template<class Func, class... Args>
		void addTask(Func func, Args... args) {
#ifdef _MANAGED
#else
			std::lock_guard<std::mutex> lock(_mtx);
			_tasks.emplace(func, args...);
			_cv.notify_one();
#endif
		}
		void setSize(size_t size);
		bool isWorking();
		void complete();
		void abort();
	};

}

#endif
