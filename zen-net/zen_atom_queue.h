#pragma once

#include <thread>
#include <queue>
#include <condition_variable>

namespace Zen {
	template<typename _Type>
	class AtomQueue {
	public:
		typedef _Type ValueType;
	protected:
		std::deque<ValueType> mQue;
		std::mutex mMutex;
		std::condition_variable_any mEmptyCondition;
		std::condition_variable_any mFullCondition;
		size_t mFullSize;
	public:
		AtomQueue()
		{
			mFullSize = (1<<16);
		}
		void setFullSize(size_t size)
		{
			mFullSize = size;
		}
		size_t getFullSize() const
		{
			return mFullSize;
		}

		void push(ValueType value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			while(mQue.size() >= mFullSize) mFullCondition.wait(mMutex);
			mQue.push_back(value);
			mEmptyCondition.notify_one();
		}
		size_t size()
		{
			return mQue.size();
		}

		ValueType pop()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			while(mQue.empty()) mEmptyCondition.wait(mMutex);
			auto v = mQue.front();
			mQue.pop_front();
			return v;
		}

		std::deque<ValueType> pop_all()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			std::deque<ValueType> e;
			std::swap(mQue, e);
			mFullCondition.notify_all();
			return e;
		}
	};
}
