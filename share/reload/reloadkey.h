#ifndef __RELOADKEY_H__
#define __RELOADKEY_H__



class GSReloadKey
{

private:
	GSReloadKey();
	~GSReloadKey();

public:
	typedef std::set<std::string>::iterator iterator;

	static GSReloadKey& GetInstance()
	{
		static GSReloadKey instance;
		return instance;
	}

	iterator begin() const
	{
		return m_reloadKey.begin();
	}

	iterator end() const
	{
		return m_reloadKey.end();
	}

	bool IsIn(const std::string& key)
	{
		return m_reloadKey.find(key) != m_reloadKey.end();
	}

	UINT32 size()
	{
		return m_reloadKey.size();
	}

private:
	std::set<std::string> m_reloadKey;
};



#endif