#ifndef _UTIL_MAP_LIST_H_
#define _UTIL_MAP_LIST_H_

#include <map>
#include <list>

template <typename T, typename Compare = std::less<T>>
class MapList
{
private:
	struct _MapData;
	struct _ListData; 

	typedef std::map<T, _MapData, Compare> MapContainer;
	typedef std::list<_ListData> ListContainer;

	struct _MapData
	{
		typename ListContainer::iterator itList;
	};

	struct _ListData
	{
		_ListData(typename MapContainer::iterator it): itMap(it) {}
		typename MapContainer::iterator itMap;
	};

public:
	void push_back(const T& val)
	{
		auto it = m_map.find(val);
		if (it != m_map.end())
		{
			return;
		}

		_MapData mapData;
		auto p = m_map.insert(std::make_pair(val, mapData));
		it = p.first;
		m_list.push_back(_ListData(it));
		it->second.itList = --m_list.end();
	}

	void push_front(const T& val)
	{
		auto it = m_map.find(val);
		if (it != m_map.end())
		{
			return;
		}

		_MapData mapData;
		auto p = m_map.insert(std::make_pair(val, mapData));
		it = p.first;
		m_list.pop_back(_ListData(it));
		it->second.itList = m_list.begin();
	}

	T pop_front()
	{
		if (m_list.empty())
		{
			return T();
		}
		_ListData l = m_list.front();
		T val = l.itMap->first;
		m_list.pop_front();
		m_map.erase(l.itMap);
		return val;
	}

	T pop_back()
	{
		if (m_list.empty())
		{
			return T();
		}
		_ListData l = m_list.back();
		T val = l.itMap->first;
		m_list.pop_back();
		m_map.erase(l.itMap);
		return val;
	}

	bool empty()
	{
		return m_list.empty();
	}

	size_t size()
	{
		return m_list.size();
	}

	void remove(const T& val)
	{
		auto it = m_map.find(val);
		if (it == m_map.end())
		{
			return;
		}
		m_list.erase(it->second.itList);
		m_map.erase(it);
	}

	void clear()
	{
		m_map.clear();
		m_list.clear();
	}

private:
	MapContainer	m_map;
	ListContainer	m_list;
};

#endif
