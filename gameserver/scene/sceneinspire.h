#ifndef __SCENEINSPIRE_H__
#define __SCENEINSPIRE_H__

namespace KKSG
{
	class InspireArg;
	class InspireRes;
}

class Scene;
class Role;
class SceneInspire
{
public:
	SceneInspire(Scene* scene);
	~SceneInspire();

	//判断是否需要鼓舞功能
	static bool NeedInspire(Scene* pScene);

	//处理鼓舞请求
	void HandlerReq(Role* pRole, const KKSG::InspireArg& oArg, KKSG::InspireRes& oRes);

	//公会擂台战鼓舞
	void GwbInspire(Role* pRole, KKSG::InspireRes& oRes);
	void GwbOnFightBegin(UINT64 roleID, UINT64 guildID);//玩家上阵即可获得累计的鼓舞效果

	INT32 GetInspireCount(UINT64 inspireID);

private:
	//roleid 发起鼓舞者 所需冷却时间
	bool InCoolDown(UINT64 roleid, UINT32 cooldown);
	UINT32 GetCoolDownTime(UINT64 roleid, UINT32 cooldown);

	//roleid 发起鼓舞者 inspireID被鼓舞者(不一定是玩家)
	void AddOneInspire(UINT64 roleid, UINT64 inspireID);

	//添加属性 addattrID（被添加属性的玩家） 
	void AddTempAttr(std::vector<UINT64>& addAttrIDs, UINT64 inspireID, bool isfirst);

	std::unordered_map<UINT64,time_t> m_inspireTime;//(发起鼓舞者)最近一次鼓舞时间
	std::unordered_map<UINT64,INT32> m_inspireCount;//(被鼓舞者（不一定是玩家）) 被鼓舞次数

	Scene* m_pScene;
};

#endif