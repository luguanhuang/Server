#ifndef _SIMPLE_TIME_H_
#define _SIMPLE_TIME_H_

#include <map>
#include <list>

template <typename KeyT, typename TimeT>
class SimpleTimer
{
public:

	void Add(KeyT key, TimeT time)
	{
		auto itKey = m_key.find(key);
		if (itKey != m_key.end())
		{
			Erase(key, itKey->second);
			itKey->second = time;
		}
		else
		{
			m_key.insert(std::make_pair(key, time));
		}
		m_time.insert(std::make_pair(time, key));
	}

	void Remove(KeyT key)
	{
		auto itKey = m_key.find(key);
		if (itKey == m_key.end())
		{
			return;
		}
		Erase(key, itKey->second);
		m_key.erase(itKey);	
	}

	void Update(TimeT now)
	{
		for (auto it = m_time.begin(); it != m_time.end(); )
		{
			if (it->first > now)
			{
				break;
			}
			KeyT key = it->second;
			if (m_cb)
			{
				m_cb(key);
			}
			m_time.erase(it++);
			m_key.erase(key);
		}
	}

	void SetCallBack(std::function<void (KeyT)> cb)
	{
		m_cb = cb;
	}

private:
	void Erase(KeyT key, TimeT time)
	{
		auto itPair = m_time.equal_range(time);	
		for (auto it = itPair.first; it != itPair.second; ++it)
		{
			if (it->second == key)
			{
				m_time.erase(it);
				return;
			}
		}
	}

private:
	std::multimap<TimeT, KeyT> m_time;
	std::unordered_map<KeyT, TimeT> m_key;
	std::function<void (KeyT)> m_cb;
};

#endif