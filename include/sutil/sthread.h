#ifndef SLIB_STHREAD_H
#define SLIB_STHREAD_H

#include "sconfig.h"
#include "sbasic/array.h"

namespace slib {

	template<>
	class Memory<std::mutex> {
	public:
		static inline char* alloc(const size_t sz) { return (char*)malloc(sz); }
		static inline void dealloc(std::mutex* ptr) { free(ptr); }
		static inline void init(std::mutex* ptr) { new(ptr) std::mutex(); }
		static inline void init(std::mutex* ptr, const std::mutex& val) {}
		static inline void init(std::mutex* ptr, std::mutex&& val) noexcept {}
		template<class... Args>
		static inline void init(std::mutex* ptr, Args... args) {}
		static inline void destroy(std::mutex* ptr) { ptr->~mutex(); }
		static inline void clear(std::mutex* ptr) { ptr->unlock(); }
		static inline void copy(std::mutex* dest, const std::mutex* src, const size_t sz) {}
		static inline void move(std::mutex** dest, std::mutex** src) {}
		static inline void shift(std::mutex* from, std::mutex* to, const size_t sz) {}
		static inline void swap(std::mutex* from, std::mutex* to) {}
	};

	namespace sutil {
		/**
		* \~english @brief Sleep function
		*/
		extern inline void sleep(sint ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
	}
	/**
	* @class SLock
	* \~english Spin lock
	*/
	class SLIB_DLL SLock {
	private:
		std::atomic<bool> _lock;
	public:
		SLock();
		SLock(const SLock& l);
		~SLock();
		SLock& operator=(const SLock& l);
		void lock();
		void unlock();
	};
	class SLIB_DLL SAutoLock {
	private:
		SLock *_lock;
	public:
		SAutoLock(SLock& l);
		~SAutoLock();
	};
	/**
	* @class SThread
	* \~english @brief Thread class
	*/
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
		SThread(const SThread& t);
		~SThread();
		template<class Func, class... Args>
		void start(Func f, Args... args) {
			_state = SThread::WORKING;
			_thread = std::thread(f, args...);
		}
		STATE state() const;
		void setState(STATE st);
		void complete();
	};
	/**
	* @class STask
	* \~english @brief Task class for thread pool
	*/
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
	/**
	* @class SWork
	* \~english @brief Thread pool
	*/
	class SLIB_DLL SWork {
	public:
	private:
		bool _run;
		size_t _count;
		Array<SThread> _threads;
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
			std::unique_lock<std::mutex> lock(_mtx);
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
