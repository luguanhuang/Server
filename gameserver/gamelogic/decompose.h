#ifndef __DECOMPOSE_H__
#define __DECOMPOSE_H__

class Role;
struct XItem;
struct DecomposeHelper
{
	void Init(const std::vector<std::pair<XItem*, UINT32>>& items, float param);
	bool Fini();

	DecomposeHelper(Role* role)
	{
		m_param = 1.0f;
		m_role = role;	
	}
	float m_param;
	Role* m_role;
	std::vector<std::pair<XItem*, UINT32>> m_items;
};

#endif
