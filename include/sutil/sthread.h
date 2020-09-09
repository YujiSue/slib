#ifndef SLIB_STHREAD_H
#define SLIB_STHREAD_H

#include "sbasic.h"

namespace slib {
	class SLIB_DLL SLock {
	private:
		std::atomic<bool> _lock;
	public:
		SLock();
		~SLock();
		void lock();
		void unlock();
	};
	class SLIB_DLL SAutoLock {
	private:
		SLock _lock;
	public:
		SAutoLock();
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
		std::thread _thread;

	public:
		SThread();
		SThread(suinteger i);
		~SThread();
		template<class Func, class... Args>
		void start(Func f, Args... args) {
			_state = SThread::WORKING;
			_thread = std::thread(f, args...);
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
		std::condition_variable _cv;
		std::mutex _mtx;

	private:
		void _init();

	public:
		SWork();
		SWork(size_t s);
		~SWork();

		template<class Func, class... Args>
		void addTask(Func func, Args... args) {
			std::lock_guard<std::mutex> lock(_mtx);
			_tasks.emplace(func, args...);
			_cv.notify_one();
		}
		void setSize(size_t size);
		bool isWorking();
		void complete();
		void abort();
	};
}

#endif
