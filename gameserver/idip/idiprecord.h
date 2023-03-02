#ifndef __IDIP_RECORD_H__
#define __IDIP_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"

class Role;

class CIdipRecord : public RoleSerializeInterface
{
public:
	CIdipRecord(Role* pRole);
	~CIdipRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void SetMessage(const std::string& strMessage);
	const std::string& GetMessage() const { return m_szMessage;}
	const KKSG::IdipPunishData* GetIdipPunishData(INT32 iType);
	void SetIdipPunishData(KKSG::IdipPunishData& oData);
	void UnDoIdipPunishData(INT32 iType);
	bool CheckZeroProFit();
	void NotifyIdipPunish(KKSG::IdipPunishData oData);
	void NotifyMessage();
	void DoNotice(const KKSG::CheckNoticeArg &roArg, KKSG::CheckNoticeRes &roRes);
	void ClickNotice(KKSG::PlatNotice notice);
	static bool CompareRule(const KKSG::PlatNotice& fir, const KKSG::PlatNotice& sec);

	bool CheckIdipAndNotify(INT32 iType, bool notify = true);
	void Update();

	void SetXinYueHint(bool hint);
	bool GetXinYueHint() { return m_XinYueHint; }

	void SetHint(UINT32 systemid, bool hint);
	void CheckHint(UINT32 systemid);
	void CheckAllHint();

	INT32 GetAdultType() { return m_AdultType; }
	void SetAdultType(INT32 adulttype);

private:
	Role* m_pRole;
	Switch m_isModify;

	std::string m_szMessage;
	std::unordered_map<INT32, KKSG::IdipPunishData> m_vPunishInfo;//type=>data
	std::vector<KKSG::PlatNotice> m_vNoticeInfo;//用来保存读取过的公告，红点判断;
	std::map<UINT32, bool> m_HintInfo;
	bool m_XinYueHint;
	INT32 m_AdultType;
};

struct EqualNotice
{
	INT32 type;
	INT32 ID;
	INT32 areaid;
	INT32 platid;
	INT32 updatetime;
	EqualNotice(INT32 iType, INT32 iID, INT32 iAreaid, INT32 iPlatID, INT32 iUpdatetime)
	{
		type = iType;
		ID = iID;
		areaid = iAreaid;
		platid = iPlatID;
		updatetime = iUpdatetime;
	}
	bool operator()(const KKSG::PlatNotice& roInfo)
	{
		return roInfo.type()==type&&roInfo.noticeid()==ID&&roInfo.areaid()==areaid&&roInfo.platid()==platid&&roInfo.updatetime()==updatetime;
	}
};
#endif
