#ifndef __MULTIINDEX_MAP_H__
#define __MULTIINDEX_MAP_H__

#include <list>
#include <unordered_map>


template <typename T, typename KeySelector1, typename KeySelector2>
class mutliindex_map
{
private:

	typedef std::list<T> ValueList;
	typedef typename ValueList::iterator Iterator;

	ValueList m_values;
	std::unordered_map<typename KeySelector1::key, Iterator> m_index1;
	std::unordered_map<typename KeySelector2::key, Iterator> m_index2;

public:

	typedef Iterator iterator;

	mutliindex_map() {}
	~mutliindex_map() {}

	void insert(const T &t)
	{
		m_values.push_back(t);
		Iterator it = m_values.end();
		--it;

		KeySelector1 k1;
		KeySelector2 k2;
		m_index1[k1.Key(t)] = it;
		m_index2[k2.Key(t)] = it;
	}

	void clear()
	{
		m_values.clear();
		m_index1.clear();
		m_index2.clear();
	}

	std::size_t size()
	{
		return m_values.size();
	}

	bool empty()
	{
		return m_values.empty();
	}

	iterator begin()
	{
		return m_values.begin();
	}

	iterator end()
	{
		return m_values.end();
	}

private:

	void erase(Iterator it)
	{
		T &t = *it;
		KeySelector1 k1;
		KeySelector2 k2;
		m_index1.erase(k1.Key(t));
		m_index2.erase(k2.Key(t));
		m_values.erase(it);
	}

	bool erase(const typename KeySelector1::key &key, const KeySelector1 &)
	{
		auto i = m_index1.find(key);
		if (i == m_index1.end())
		{
			return false;
		}

		erase(i->second);
		return true;
	}

	bool erase(const typename KeySelector2::key &key, const KeySelector2 &)
	{
		auto i = m_index2.find(key);
		if (i == m_index2.end())
		{
			return false;
		}

		erase(i->second);
		return true;
	}

	T *find(const typename KeySelector1::key &key, const KeySelector1&) 
	{
		auto i = m_index1.find(key);
		if (i == m_index1.end())
			return NULL;
		return &(*i->second);
	}

	T *find(const typename KeySelector2::key &key, const KeySelector2&)
	{
		auto i = m_index2.find(key);
		if (i == m_index2.end())
			return NULL;
		return &(*i->second);
	}

public:

	template<typename KeySelectorType>
	bool erase(const typename KeySelectorType::key &key)
	{
		return erase(key, KeySelectorType());
	}

	template<typename KeySelectorType>
	T *find(const typename KeySelectorType::key &key)
	{
		return find(key, KeySelectorType());
	}
};


template <typename T, typename KeySelector1, typename KeySelector2>
class mutliindex_map_ptr
{
private:

	typedef std::list<T*> ValueList;
	typedef typename ValueList::iterator Iterator;

	ValueList m_values;
	std::unordered_map<typename KeySelector1::key, Iterator> m_index1;
	std::unordered_map<typename KeySelector2::key, Iterator> m_index2;

public:

	typedef Iterator iterator;

	mutliindex_map_ptr() {}
	~mutliindex_map_ptr() {}

	void insert(T *t)
	{
		m_values.push_back(t);
		Iterator it = m_values.end();
		--it;

		KeySelector1 k1;
		KeySelector2 k2;
		m_index1[k1.Key(t)] = it;
		m_index2[k2.Key(t)] = it;
	}

	void clear()
	{
		m_values.clear();
		m_index1.clear();
		m_index2.clear();
	}

	std::size_t size()
	{
		return m_values.size();
	}

	bool empty()
	{
		return m_values.empty();
	}

	iterator begin()
	{
		return m_values.begin();
	}

	iterator end()
	{
		return m_values.end();
	}

private:

	void erase(Iterator it)
	{
		T *t = *it;
		KeySelector1 k1;
		KeySelector2 k2;
		m_index1.erase(k1.Key(t));
		m_index2.erase(k2.Key(t));
		m_values.erase(it);
	}

	bool erase(const typename KeySelector1::key &key, const KeySelector1 &)
	{
		auto i = m_index1.find(key);
		if (i == m_index1.end())
		{
			return false;
		}

		erase(i->second);
		return true;
	}

	bool erase(const typename KeySelector2::key &key, const KeySelector2 &)
	{
		auto i = m_index2.find(key);
		if (i == m_index2.end())
		{
			return false;
		}

		erase(i->second);
		return true;
	}

	T *find(const typename KeySelector1::key &key, const KeySelector1&) 
	{
		auto i = m_index1.find(key);
		if (i == m_index1.end())
			return NULL;
		return *i->second;
	}

	T *find(const typename KeySelector2::key &key, const KeySelector2&)
	{
		auto i = m_index2.find(key);
		if (i == m_index2.end())
			return NULL;
		return *i->second;
	}

public:

	template<typename KeySelectorType>
	bool erase(const typename KeySelectorType::key &key)
	{
		return erase(key, KeySelectorType());
	}

	template<typename KeySelectorType>
	T *find(const typename KeySelectorType::key &key)
	{
		return find(key, KeySelectorType());
	}
};


#endif
