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
	UINT32 m_uType;//����ѫ�¶Եķ�ӡtype
	UINT32 m_uSelfCollectCount;//���˹���ѫ����
	INT32 m_iSelfAwardCountIndex;//��ǰ�Ѿ���ȡ���������±꣨-1��û����ȡ����
	UINT32 m_uLevelSealButtonStatus;
	bool m_bLastLevelSealStatus;//����ķ�ӡ״̬

};
#endif
