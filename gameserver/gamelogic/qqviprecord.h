#ifndef _QQ_VIP_RECORD_H_
#define _QQ_VIP_RECORD_H_

#include "unit/roleserilizeinterface.h"
#include "pb/enum.pb.h"

class Role;

class QQVipRecord : public RoleSerializeInterface
{
public:

	QQVipRecord(Role *pRole);
	~QQVipRecord();
	
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void FromKKSG(const KKSG::QQVipInfo& qqVip);
	void ToKKSG(KKSG::QQVipInfo& qqVip);
	void FillClientInfo(KKSG::QQVipInfoClient& info);

	void CheckNewBieGift();
	void SetDirty() { m_dirty.Set(); }

	bool IsVip();
	bool IsSVip();
	bool IsYearVip();
	bool IsXinYueVip();

	void SetIsVip(bool isVip) { m_isVip = isVip; m_dirty.Set();}
	void SetIsSvip(bool isSVip) { m_isSVip = isSVip; m_dirty.Set();}
	void SetIsYearSvip(bool isVip) { m_isYearVip = isVip; m_dirty.Set();}
	void SetIsXinYueVip(bool isVip) { m_isXinYueVip = isVip; m_dirty.Set(); }

	void SetVipTime(UINT32 start, UINT32 end);
	void SetSVipTime(UINT32 start, UINT32 end);
	void SetYearVipTime(UINT32 start, UINT32 end);

private:
	Switch m_dirty;
//	UINT32 m_lastUpdateTime;
	Role *m_role;

	bool m_isVip;
	bool m_isSVip;
	bool m_isYearVip;
	UINT32 m_vipStart;
	UINT32 m_vipEnd;
	UINT32 m_SVipStart;
	UINT32 m_SVipEnd;
	UINT32 m_yearVipStart;
	UINT32 m_yearVipEnd;

	bool m_VipNewBieRewarded;
	bool m_SVipNewBieRewarded;

	bool m_isXinYueVip;
};

#endif