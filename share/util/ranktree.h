#ifndef	_RANKTREE_H__
#define _RANKTREE_H__

#include "XRandom.h"

// author : glq 2016/7/29

// Warning: If you use a pointer as _Key, DO NOT CHANGE the data in pointer when you get the pointer from RankTree.
// Otherwise it will cause the fact that the tree can't find the pointer when you want to erase the key.
// You can change it after you erase it. Please Remember!

template<typename _Key, typename _Compare = std::less<_Key>, typename _ID = UINT64, typename _CompareID = std::less<_ID> >
class RankTree // use treap as balanced tree
{
public:
	RankTree(const _Compare& _comp = _Compare());
	~RankTree();

	void Clear();

	// if it has this id already, it will replace with new one, 
	// so don't use it when use pointer as _Key, it may cause memory leak
	void Update(const _Key& key, const _ID& id);

	// if it has this id already, it's failed to insert, and return (UINT32)(-1)
	UINT32 Insert(const _Key& key, const _ID& id);

	// 0 means success , (UINT32)(-1) means id does not exist
	UINT32 Erase(const _ID& id);

	bool CanFind(const _ID& id) const;

	//  (UINT32)(-1) means id does not exist
	UINT32 GetRankByID(const _ID& id); 

	// 0 means success, (UINT32)(-1) means rank > size of tree
	UINT32 GetKeyByRank(UINT32 rank, _Key& key); 

	// 0 means success, (UINT32)(-1) means id does not exist
	UINT32 GetKeyByID(const _ID& id, _Key& key); 

	UINT32 GetSize() const;
	UINT32 GetTreeHeight() const;

	void GetAllKey(std::vector<_Key>& vec);
	void GetSomeTopKey(int count, std::vector<_Key>& vec);	

private:
	std::map<_ID, _Key, _CompareID> m_map_id_to_key;
	_Compare key_compare;

	/*struct Combine_Key
	{
		_Key key;
		_ID id;
		_Compare* key_comp;
		Combine_Key(_Key _key, _ID _id, _Compare* _key_comp):key(_key), id(_id), key_comp(_key_comp){}
		bool operator < (const Combine_Key& x) const
		{
			if(!(*key_comp)(key, x.key) && !(*key_comp)(x.key, key)) return id < x.id;
			return (*key_comp)(key, x.key);
		}
	};*/

	struct TreapNode
	{
		TreapNode(const _Key& _key, _Compare* _key_comp):m_key(_key), key_comp(_key_comp)
		{
			son[0] = son[1] = NULL;
			m_randWeight = XRankTreeRandom::randInt();
			m_size = 1;
		}
		TreapNode* son[2];

		UINT32 m_randWeight;
		int m_size;
		_Key m_key;
		_Compare* key_comp;

		int Compare(const _Key& x) const
		{
			if(!(*key_comp)(x, m_key) && !(*key_comp)(m_key, x)) return -1;
			return (*key_comp)(m_key, x);
		}

		void MainTain()
		{
			m_size = 1;
			if(son[0] != NULL) m_size += son[0]->m_size;
			if(son[1] != NULL) m_size += son[1]->m_size;
		}
	};

	TreapNode* root;

	void rotate(TreapNode* &o, int d) // d = 0 turn left, d = 1 turn right
	{
		TreapNode* k = o->son[d^1];
		o->son[d^1] = k->son[d];
		k->son[d] = o;
		o->MainTain();
		k->MainTain();
		o = k;
	}

	bool find(TreapNode* o, const _Key& key) const
	{
		while(o != NULL)
		{
			int d = o->Compare(key);
			if(d == -1) return true;
			o = o->son[d];
		}
		return false;
	}

	int insert(TreapNode* &o, const _Key& key)
	{
		if(o == NULL) o = new TreapNode(key, &key_compare);
		else
		{
			if(!key_compare(key, o->m_key) && !key_compare(o->m_key, key))
			{
				LogError("insert same key, key_compare may error!");
				return -1;
			}
			int d = key_compare(key, o->m_key) ? 0 : 1;
			if(-1 == insert(o->son[d], key))
			{
				return -1;
			}
			if(o->m_randWeight < o->son[d]->m_randWeight) rotate(o, d^1);
		}
		o->MainTain();
		return 0;
	}

	void remove(TreapNode* &o, const _Key& key)
	{
		if(o == NULL) return;
		int d = o->Compare(key);
		if(d == -1)
		{
			TreapNode* u = o;
			if(o->son[0] != NULL && o->son[1] != NULL)
			{
				int d2 = o->son[0]->m_randWeight > o->son[1]->m_randWeight ? 1 : 0;
				rotate(o, d2);
				remove(o->son[d2], key);
			}
			else
			{
				if(o->son[0] == NULL) o = o->son[1];
				else o = o->son[0];
				delete u;
			}
		}
		else remove(o->son[d], key);
		if(o != NULL) o->MainTain();
	}

	int getRank(TreapNode* o, const _Key& key) const //please use it after confirm the key is really in tree
	{
		if(NULL == o)
		{
			LogError("ranktree data wrong");
			return -1;
		}
		int d = o->Compare(key);
		int leftSize = o->son[0] == NULL ? 0 : o->son[0]->m_size;
		if(d == -1)
		{
			return leftSize + 1;
		}
		if(d == 0) return getRank(o->son[d], key);
		return getRank(o->son[d], key) + leftSize + 1;
	}

	TreapNode* getNodeByRank(TreapNode* o, int rank) const //please use it after confirm rank <= the size of tree
	{
		int leftSize = o->son[0] == NULL ? 0 : o->son[0]->m_size;
		if(rank == leftSize + 1)
		{
			return o;
		}
		if(rank < leftSize + 1) return getNodeByRank(o->son[0], rank);
		else return getNodeByRank(o->son[1], rank - leftSize - 1);
	}

	void getSomeNode(TreapNode* o, int rank, int count, std::vector<_Key>& vec)
	{
		if(o->son[0] != NULL) getSomeNode(o->son[0], rank, count, vec);
		int leftSize = o->son[0] == NULL ? 0 : o->son[0]->m_size;
		if(rank + leftSize >= count) return;
		vec.push_back(o->m_key);
		if(rank + leftSize + 1 == count) return;
		if(o->son[1] != NULL) getSomeNode(o->son[1], rank + leftSize + 1, count, vec);
	}

	UINT32 getTreeHeight(TreapNode* o) const
	{
		if(NULL == o) return 0;
		UINT32 leftHeight = getTreeHeight(o->son[0]);
		UINT32 rightHeight = getTreeHeight(o->son[1]);
		if(leftHeight < rightHeight) return 1 + rightHeight;
		else return 1 + leftHeight;
	}

	void removeTree(TreapNode* &o)
	{
		if(o == NULL) return;
		removeTree(o->son[0]);
		removeTree(o->son[1]);
		delete o;
		o = NULL;
	}
};

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
RankTree<_Key, _Compare, _ID, _CompareID>::RankTree(const _Compare& _comp):key_compare(_comp)
{
	root = NULL;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
RankTree<_Key, _Compare, _ID, _CompareID>::~RankTree()
{
	Clear();
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
void RankTree<_Key, _Compare, _ID, _CompareID>::Clear()
{
	m_map_id_to_key.clear();
	removeTree(root);
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
bool RankTree<_Key, _Compare, _ID, _CompareID>::CanFind(const _ID& id) const
{
	if(m_map_id_to_key.find(id) == m_map_id_to_key.end())
	{
		return false;
	}
	return true;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::Insert(const _Key& key, const _ID& id)
{
	if(CanFind(id))
	{
		return (UINT32)(-1);
	}
	if(-1 == insert(root, key))
	{
		return (UINT32)(-1);
	}
	m_map_id_to_key[id] = key;	

	return 0;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::Erase(const _ID& id)
{
	if(false == CanFind(id))
	{
		return (UINT32)(-1);
	}
	const _Key& key = m_map_id_to_key[id];
	remove(root, key);
	m_map_id_to_key.erase(id);

	return 0;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
void RankTree<_Key, _Compare, _ID, _CompareID>::Update(const _Key& key, const _ID& id)
{
	if(CanFind(id))
	{
		Erase(id);
	}
	Insert(key, id);
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::GetRankByID(const _ID& id)
{
	if(false == CanFind(id))
	{
		return (UINT32)(-1);
	}
	const _Key& key = m_map_id_to_key[id];
	return (UINT32)getRank(root, key);
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::GetKeyByRank(UINT32 rank, _Key& key)
{
	if(GetSize() < rank || 0 == rank)
	{
		return (UINT32)(-1);
	}
	key = getNodeByRank(root, rank)->m_key;
	return 0;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::GetKeyByID(const _ID& id, _Key& key)
{
	if(m_map_id_to_key.find(id) == m_map_id_to_key.end())
	{
		return (UINT32)(-1);
	}
	key = m_map_id_to_key[id];
	return 0;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::GetSize() const
{
	return root == NULL ? 0 : root->m_size;
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
UINT32 RankTree<_Key, _Compare, _ID, _CompareID>::GetTreeHeight() const
{
	return getTreeHeight(root);
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
void RankTree<_Key, _Compare, _ID, _CompareID>::GetAllKey(std::vector<_Key>& vec)
{
	if(root == NULL) return;
	getSomeNode(root, 0, GetSize(), vec);
}

template<typename _Key, typename _Compare, typename _ID, typename _CompareID>
void RankTree<_Key, _Compare, _ID, _CompareID>::GetSomeTopKey(int count, std::vector<_Key>& vec)
{
	if(root == NULL) return;
	getSomeNode(root, 0, count, vec);
}

#endif