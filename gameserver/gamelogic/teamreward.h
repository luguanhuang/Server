#ifndef _TEAMREWARD_H_
#define _TEAMREWARD_H_

#include <vector>
#include <unordered_map>
#include "cvsreader.h"
#include "item.h"

#define BOXSIZE 4

struct BoxRewardInfo
{
	UINT32 type;
	ItemDesc item;
	bool canget;

	BoxRewardInfo()
	{
		type = 0;
		canget = false;
	}
};

struct BoxDropConf
{
	UINT32 quality;
	UINT32 per;
	UINT32 index;

	BoxDropConf()
	{
		quality = 0;
		per = 0;
		index = 0;
	}
};

struct SceneInfo;
struct RoleBoxRewardInfo
{
	bool isrobot;
	UINT64 roleid;
	UINT32 rolelevel;
	SceneInfo* sceneinfo;

	UINT32 star;
	std::vector<BoxDropConf> confs;
	
	bool isgive;
	UINT32 index;
	UINT32 peer;
	UINT32 randomcount;
	std::vector<BoxRewardInfo> boxs;

	bool Select(UINT32 _index);
	bool CanPeer();
	bool Peer(UINT32 _index);
	BoxRewardInfo* GetItem(UINT32 _index);

	void RandomOne(UINT32 _index);
	void RandomLeft();

	void Give();

	RoleBoxRewardInfo()
	{
		isrobot = false;
		isgive = false;
		randomcount = 0;
		star = 0;
		index = 0;
		peer = 0;
		roleid = 0;
		rolelevel = 0;
		sceneinfo = NULL;

		boxs.resize(BOXSIZE);
	}
};

class Scene;
class Role;
struct SceneInfo;
class BagGiveItemTransition;
class TeamSelectReward
{
public:
	TeamSelectReward(Scene* scene);
	~TeamSelectReward();

	void OnRoleLeave(Role* role);

	///> ·Å´ó¾µ
	static bool ValidIndex(UINT32 index);

	bool Select(UINT64 roleID, UINT32 index);
	RoleBoxRewardInfo* GetRoleBoxInfo(UINT64 roleID);

	void Finish(bool normal = true);
	bool IsAllSelected() { return AllSelected(); }

	void OnSetRank(UINT64 roleid, UINT32 rolelevel, UINT32 star, bool isrobot);

	void End();
	void EndOne(UINT64 roleid, RoleBoxRewardInfo& boxinfo);

	std::map<UINT64, RoleBoxRewardInfo>* GetAllBox() { return &m_roleSelect; }

	void FillRobotBox(UINT64 robotid, const RoleBoxRewardInfo& boxinfo);

	static RoleBoxRewardInfo FillBox(SceneInfo* info, UINT32 star, UINT64 roleid, UINT32 rolelv);

private:
	bool AllSelected();

	bool   m_isallselected;
	Scene* m_scene;
	std::map<UINT64, RoleBoxRewardInfo> m_roleSelect;
};

#endif 
