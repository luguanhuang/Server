#ifndef _UTIL_TIME_LIST_H_
#define _UTIL_TIME_LIST_H_

#include <map>
#include <list>

// 固定时长定时器
template <typename KeyType, typename TimeType>
class TimeList
{
public:
	typedef std::function<void (const KeyType&)> TimeOutCallBack;

private:
	struct _MapData;
	struct _ListData; 

	typedef std::map<KeyType, _MapData> MapContainer;
	typedef std::list<_ListData> ListContainer;

	struct _MapData
	{
		typename ListContainer::iterator itList;
	};

	struct _ListData
	{
		_ListData(typename MapContainer::iterator it, TimeType t): itMap(it), time(t) {}
		typename MapContainer::iterator itMap;
		TimeType time;
	};

public:
	void SetTimeOut(const TimeType& time)
	{
		m_timeout = time;
	}

	void SetCallBack(TimeOutCallBack cb)
	{
		m_cb = cb;
	}

	void Add(const KeyType& key, const TimeType& now)
	{
		_MapData mapData;
		auto p = m_map.insert(std::make_pair(key, mapData));
		auto it = p.first;
		if (p.second == false)
		{
			m_list.erase(it->second.itList);
		}
		m_list.push_back(_ListData(it, now + m_timeout));
		it->second.itList = --m_list.end();
	}

	void Remove(const KeyType& key)
	{
		auto it = m_map.find(key);
		if (it == m_map.end())
		{
			return;
		}
		m_list.erase(it->second.itList);
		m_map.erase(it);
	}

	void Update(const TimeType& now)
	{
		for (typename ListContainer::iterator it = m_list.begin(); it != m_list.end(); )
		{
			if (now >= it->time)
			{
				if (m_cb)
				{
					m_cb(it->itMap->first);
				}
				m_map.erase(it->itMap);
				it = m_list.erase(it);
			}
			else
			{
				break;
			}
		}
	}

private:
	TimeType		m_timeout;
	MapContainer	m_map;
	ListContainer	m_list;
	TimeOutCallBack m_cb;
};

#endif