#ifndef __PLAT_NOTICE_H__
#define __PLAT_NOTICE_H__

#include "define/platnotice_def.h"
#include "idip/idip.h"

typedef std::map<std::string, KKSG::PlatNotice> CPlatNoticeInfoMap;
class CPlatNoticeMgr
{
	CPlatNoticeMgr();
	~CPlatNoticeMgr();

	DECLARE_SINGLETON(CPlatNoticeMgr)

public:
	bool Init();
	void Uninit();
	void UpdateNotice(KKSG::PlatNotice& notice);
	bool ClickNoticeCheck(const KKSG::PlatNotice& notice);
	const CPlatNoticeInfoMap GetNoticeList() { return m_oNoticeList;}
	void SaveNotice(const KKSG::PlatNotice& notice);
	UINT32 GetAreaID(KKSG::LoginType loginType);
	void GetNotice(KKSG::LoginType loginType, KKSG::PlatType platId, std::vector<KKSG::PlatNotice>& list);
	void GetNotice(UINT32 uAreaID, UINT32 uPlatID, 	std::vector<KKSG::PlatNotice>& list);
	KKSG::PlatNotice* FindNotice(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID, UINT32 uID);
	void HintNotifyClient(INT32 platid, UINT32 areaid);
private:
	bool ReadAllNoticeFromDB();
	std::string GetKey(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID, UINT32 uID);
	CPlatNoticeInfoMap m_oNoticeList;
};
#endif

