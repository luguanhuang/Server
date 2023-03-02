#ifndef __ATLAS_H__
#define __ATLAS_H__

#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"


class Role;
class CombatAttribute;
struct ItemChangeAttr; 
class CAtlasSys : public RoleSerializeInterface
{

public:
	CAtlasSys();
	CAtlasSys(Role* pRole);
	~CAtlasSys();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo); 
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	UINT32 ActivatAtlas(UINT32 teamId);  // 激活
	void GetAllAttr(std::vector<ItemChangeAttr>& attrsVec); // 获取全部属性
	void ApplyEffect(CombatAttribute *roleattrs);  // 加入图鉴属性
	void BreakCard(UINT32 itemId, UINT32 itemNum); // 分解卡
	void SynAtlasAttr();
	UINT32 AutoBreakCard(const std::set<UINT32>& qualitSet, UINT32 groupId);
	UINT32 GetAtlasPowerPoint();
	bool UpStar(UINT32 groupId);  // 升星
	UINT32 GetFinishId(UINT32 groupId);
	void openGroup(UINT32 groupId);
	void Logout(const std::string& trans);
	
private:
	inline bool _IsChange() { return m_switch.TestAndReset(); }
	inline void _SetChange() { m_switch.Set(); }
	bool _IsGroupOpen(UINT32 groupId);
	bool _HasALreadyTeam(UINT32 teamId);
	UINT32 _GetFinishId(UINT32 groupId);
	bool _IsGroupFinish(UINT32 groupId);
	void DoTxLog(UINT32 groupId, UINT32 teamId, UINT32 op=0, const std::string& trans="");


private:
	Switch m_switch;
	Role* m_pRole;
	std::map<UINT32, std::set<UINT32> > m_groupMap;
	std::map<UINT32, UINT32> m_finishRcd;
};









#endif
