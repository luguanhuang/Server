#ifndef __PUNISHMGR_H__
#define __PUNISHMGR_H__
#include "role/role.h"

#define MAX_LOCK_ROLE_TIME 30 //临时锁号的最大时间; 

//用户惩罚(禁言、封号、临时封禁)处理
class CPunishMgr : public ITimer
{
	CPunishMgr();
	~CPunishMgr();
	DECLARE_SINGLETON(CPunishMgr);

public:
	bool Init();
	void Uninit();

	bool DoPunishUser(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear=false);//封号、禁言、临时处罚;
	bool UnDoPunishUser(UINT64 qwRoleID, INT32 nType);//解除;
	void DoPunishTemp(UINT64 qwRoleID);//临时封禁锁住帐号(主要针对有些操作未完成时禁止用户登录);

	bool IsPunish(UINT64 qwRoleID, INT32 nType);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void GetPunishList(INT32 nType, std::vector<KKSG::PunishData>& vList);
	bool GetPunishInfo(UINT64 qwRoleID, INT32 nType, KKSG::PunishData& oData);
	bool CheckPunishAndNotify(CRole* pRole, UINT64 qwID, INT32 nType);//qwid可能是公会id;

	bool PunishRank(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear=false);		//禁离线榜;
	bool PunishOfflineRank(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear=false);//禁排行榜;
	void UnPunishALLRank(UINT64 qwRoleID);

	void UpdatePunishRankData();//起服的时候将之前封禁时的内容放到排行榜中;

private:
	bool ReadAllPunishListFromDB();
	std::string GetKey(UINT64 qwRoleID, INT32 nType);
	bool IsExpireTime(UINT32 uBanTime, UINT32 uPunishDate);
	bool OperRole(UINT64 qwRoleID, INT32 iType, KKSG::PunishData& oData, bool isClear=false);

private:
	typedef std::unordered_map<std::string, KKSG::PunishData> CPunishInfoMap; //封号，roleid/guildid=>PunishData
	typedef std::unordered_map<INT32, INT32> CIdipRankTypeMap;
	typedef std::unordered_map<INT32, INT32> CIdipRankOfflineTypeMap;

	CPunishInfoMap m_oPunishInfo;
	CIdipRankTypeMap m_vIdipRankType;
	CIdipRankOfflineTypeMap m_vIdipRankOfflineType;

	HTIMER m_TimeHandler;
};
#endif