#ifndef _H__Lru_Cache_H__
#define _H__Lru_Cache_H__



template <typename _kty, typename _vty>
class CLruCache
{
public:
	CLruCache()
		:m_dwMaxSize(1)
	{
		m_oListHead.m_poNext = &m_oListHead;
		m_oListHead.m_poPrev = &m_oListHead;
	}

	~CLruCache()
	{
	}

	void Init(UINT32 dwMaxSize)
	{
		if(dwMaxSize > 0)
		{
			m_dwMaxSize = dwMaxSize;
		}
	}

	void UnInit()
	{
		LruNode* poNode = &m_oListHead;
		while(poNode->m_poNext != poNode)
		{
			LruNode* poDel = poNode->m_poNext;	
			_RemoveNode(poDel);
			delete poDel;
		}
		m_oNodeMap.clear();
	}

	_vty* Get(_kty key)
	{
		typename CLruNodeMap::iterator it = m_oNodeMap.find(key);
		if(it == m_oNodeMap.end())
		{
			return NULL;
		}

		LruNode* poNode = it->second;
		_RemoveNode(poNode);
		_AddNode(&m_oListHead, poNode);
		return &poNode->m_oVal;
	}

	void Put(_kty key, const _vty& val)
	{
		typename CLruNodeMap::iterator it = m_oNodeMap.find(key);
		if(it != m_oNodeMap.end())
		{
			return;
		}

		if(Full())
		{
			_kty key_del = GetLast();
			m_oNodeMap.erase(key_del);
			LruNode* poDel = m_oListHead.m_poPrev;
			_RemoveNode(poDel);
			delete poDel;
		}

		LruNode* poNew = new LruNode();
		poNew->m_oKey = key;
		poNew->m_oVal = val;
		_AddNode(&m_oListHead, poNew);
		m_oNodeMap.insert(std::make_pair(key, poNew));
	}

private:
	bool Full() const
	{
		return m_oNodeMap.size() >= m_dwMaxSize;
	}

	_kty GetLast()
	{
		if(m_oListHead.m_poPrev == NULL || m_oListHead.m_poPrev == &m_oListHead)
		{
			return _kty();
		}
		return m_oListHead.m_poPrev->m_oKey;
	}

private:
	struct LruNode
	{
		LruNode*	m_poNext;
		LruNode*	m_poPrev;
		_kty		m_oKey;
		_vty		m_oVal;

		LruNode()
			:m_poNext(NULL)
			,m_poPrev(NULL)
		{
		}
		~LruNode()
		{
		}
	};

private:
	void _InitNode(LruNode* poNode)
	{
		if(poNode == NULL) return;

		poNode->m_poNext = NULL;
		poNode->m_poPrev = NULL;
	}

	void _AddNode(LruNode* poAfter, LruNode* poNew)
	{
		if(poAfter == NULL || poNew == NULL) return;

		poNew->m_poNext = poAfter->m_poNext;
		poAfter->m_poNext->m_poPrev = poNew;
		poAfter->m_poNext = poNew;
		poNew->m_poPrev = poAfter;
	}

	void _RemoveNode(LruNode* poNode)
	{
		if(poNode == NULL) return;

		poNode->m_poNext->m_poPrev = poNode->m_poPrev;
		poNode->m_poPrev->m_poNext = poNode->m_poNext;
	}

private:
	typedef std::map<_kty, LruNode*> CLruNodeMap;
	UINT32		m_dwMaxSize;
	LruNode		m_oListHead;
	CLruNodeMap m_oNodeMap;
};


#endif
