#ifndef __PUNISHMGR_H__
#define __PUNISHMGR_H__
#include "role/role.h"

#define MAX_LOCK_ROLE_TIME 30 //��ʱ���ŵ����ʱ��; 

//�û��ͷ�(���ԡ���š���ʱ���)����
class CPunishMgr : public ITimer
{
	CPunishMgr();
	~CPunishMgr();
	DECLARE_SINGLETON(CPunishMgr);

public:
	bool Init();
	void Uninit();

	bool DoPunishUser(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear=false);//��š����ԡ���ʱ����;
	bool UnDoPunishUser(UINT64 qwRoleID, INT32 nType);//���;
	void DoPunishTemp(UINT64 qwRoleID);//��ʱ�����ס�ʺ�(��Ҫ�����Щ����δ���ʱ��ֹ�û���¼);

	bool IsPunish(UINT64 qwRoleID, INT32 nType);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void GetPunishList(INT32 nType, std::vector<KKSG::PunishData>& vList);
	bool GetPunishInfo(UINT64 qwRoleID, INT32 nType, KKSG::PunishData& oData);
	bool CheckPunishAndNotify(CRole* pRole, UINT64 qwID, INT32 nType);//qwid�����ǹ���id;

	bool PunishRank(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear=false);		//�����߰�;
	bool PunishOfflineRank(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear=false);//�����а�;
	void UnPunishALLRank(UINT64 qwRoleID);

	void UpdatePunishRankData();//�����ʱ��֮ǰ���ʱ�����ݷŵ����а���;

private:
	bool ReadAllPunishListFromDB();
	std::string GetKey(UINT64 qwRoleID, INT32 nType);
	bool IsExpireTime(UINT32 uBanTime, UINT32 uPunishDate);
	bool OperRole(UINT64 qwRoleID, INT32 iType, KKSG::PunishData& oData, bool isClear=false);

private:
	typedef std::unordered_map<std::string, KKSG::PunishData> CPunishInfoMap; //��ţ�roleid/guildid=>PunishData
	typedef std::unordered_map<INT32, INT32> CIdipRankTypeMap;
	typedef std::unordered_map<INT32, INT32> CIdipRankOfflineTypeMap;

	CPunishInfoMap m_oPunishInfo;
	CIdipRankTypeMap m_vIdipRankType;
	CIdipRankOfflineTypeMap m_vIdipRankOfflineType;

	HTIMER m_TimeHandler;
};
#endif