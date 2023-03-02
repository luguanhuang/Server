#ifndef __LEVELSEAL_RECORD_H__
#define __LEVELSEAL_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include "unit/role.h"

class CLevelSealRecord : public RoleSerializeInterface
{
public:
	CLevelSealRecord(Role* role);
	virtual ~CLevelSealRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	void Change();
	UINT32 GetButtonStatus();
	void SetButtonStatus(UINT32 uStatus);
	bool GetLastStatus();
	void SetLastStatus(bool bStatus);
	UINT32 GetSelfCollectCount();
	INT32 GetSelfAwardCountIndex();
	UINT32 GetType();
	void CheckType(INT32 iType);
	void AddSelfCollectCount(UINT32 Count);
	void SetSelfAwardCountIndex(INT32 iIndex);
	UINT64 GetOverFlowExp();
	void SetOverFlowExp(UINT64 exp);

private:
	Role* m_pRole;
	Switch m_ismodify;
	UINT32 m_uType;//贡献勋章对的封印type
	UINT32 m_uSelfCollectCount;//个人贡献勋章数
	INT32 m_iSelfAwardCountIndex;//当前已经领取进度数据下标（-1：没有领取过）
	UINT32 m_uLevelSealButtonStatus;
	bool m_bLastLevelSealStatus;//最近的封印状态

};
#endif
