#pragma once

#include <thread>
#include <queue>
#include <map>
#include <set>
#include <condition_variable>

namespace Zen {
	template<typename _Type>
	class AtomQue
	{
	public:
		typedef _Type Type;
		typedef std::deque<Type> Que;
		typedef typename Que::iterator Iterator;
	protected:
		Que mQue;
		std::mutex mMutex;
	public:
		AtomQue() {}
		
		void push_front(Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mQue.push_front(value);
		}
		void push_back(Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mQue.push_back(value);
		}
		void pop_front()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mQue.pop_front();
		}
		void pop_back()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mQue.pop_back();
		}
		void clear()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mQue.clear();
		}
		void execute(std::function<void(Que & que)> run)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			run(mQue);
		}
	};
	
	template<typename _Type>
	class AtomConditionQue {
	public:
		typedef _Type Type;
	protected:
		std::deque<Type> mQue;
		std::mutex mMutex;
		std::condition_variable_any mEmptyCondition;
		std::condition_variable_any mFullCondition;
		size_t mMaxSize;
	public:
		AtomConditionQue()
		{
			mMaxSize = (1u<<16);
		}
		
		void setMaxSize(size_t size)
		{
			mMaxSize = size;
		}
		
		size_t getMaxSize() const
		{
			return mMaxSize;
		}
		
		size_t size()
		{
			return mQue.size();
		}

		void push(Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			while(mQue.size() >= mMaxSize) mFullCondition.wait(mMutex);
			mQue.push_back(value);
			mEmptyCondition.notify_one();
		}

		Type pop()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			while(mQue.empty()) mEmptyCondition.wait(mMutex);
			auto v = mQue.front();
			mQue.pop_front();
			return v;
		}

		std::deque<Type> pop_all()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			std::deque<Type> e;
			std::swap(mQue, e);
			mFullCondition.notify_all();
			return e;
		}
	};
	
	template<typename _Key, typename _Type>
	class AtomMap
	{
	public:
		typedef _Key Key;
	 	typedef _Type Type;
		typedef std::map<Key, Type> Map;
		typedef typename Map::iterator Iterator;
	protected:
		Map mMap;
		std::mutex mMutex;
	public:
		AtomMap() {
		
		}
		
		size_t size()
		{
			return mMap.size();
		}
		
		bool find(Key const & key, std::function<void(Iterator)> run)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			auto iter = mMap.find(key);
			if(iter == mMap.end()) return false;
			run(iter);
			return true;
		}
		
		bool erase(Key const & key)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			auto i = mMap.find(key);
			if(i == mMap.end()) return false;
			mMap.erase(i);
			return true;
		}
		
		void insert(Key const & key, Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mMap[key] = value;
		}
		
		void clear()
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mMap.clear();
		}
		
		void execute(std::function<void(Map & map)> run)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			run(mMap);
		}
	};
	
	template<typename _Type>
	class AtomSet
	{
	public:
		typedef _Type Type;
		typedef std::set<Type> Set;
	protected:
		Set mSet;
		std::mutex mMutex;
	public:
		AtomSet()
		{
		}
		void insert(Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mSet.insert(value);
		}
		void execute(std::function<void(Set & set)> run)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			run(mSet);
		}
		bool find(Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			return (mSet.find(value) != mSet.end());
		}
		bool erase(Type const & value)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			auto i = mSet.find(value);
			if(i == mSet.end()) return false;
			mSet.erase(i);
			return true;
		}
	};
}
