#ifndef __ROLEVIEW_H__
#define __ROLEVIEW_H__

enum ViewType
{
	View_Friend = 1,
	View_Guild = 2, 
	View_FriendValue = 3,
	//View_Team = 4,
	//View_First = 5,

	View_Dummy,
};

struct ViewUnit
{
	UINT64 roleid;
	UINT32 mask;
	UINT32 value;

	ViewUnit()
	{
		roleid = 0;
		mask = 0;
		value = 0;
	}

	bool operator < (const ViewUnit& other) const
	{
		if (mask == other.mask)
		{
			return roleid < other.roleid;
		}
		else
		{
			return mask > other.mask;
		}
	}
};

class Role;
class RoleView
{
public:
	RoleView(Role* role);
	~RoleView();
	
	void Add(UINT64 roleid, ViewType type, UINT32 value = 0);
	void Del(UINT64 roleid, ViewType type);

	void SetMustSee(UINT64 roleid) { m_mustsee.insert(roleid); }
	void ClearMustSee() { m_mustsee.clear(); }
	const std::set<UINT64>& GetMustSee()  { return m_mustsee; }

	void SetBindRole(UINT64 roleid, bool isneedpre) { m_bindrole= roleid; m_isneedpre = isneedpre; }
	inline UINT64 GetBindRole()  { return m_bindrole; }
	inline bool GetIsNeedPre()  { return m_isneedpre; }

public:	
	Role* m_role;
	///> 一定能看到的角色
	std::set<UINT64> m_mustsee;
	///> 和这人绑定，在别人视野里同时消失，同时出现
	bool m_isneedpre;
	UINT64 m_bindrole;

	std::set<ViewUnit> m_views;
	std::unordered_map<UINT64, std::set<ViewUnit>::iterator> m_role2viewunit;
};

#endif