#ifndef __PLATNOTICE_MGR_H__
#define __PLATNOTICE_MGR_H__

#include "define/platnotice_def.h"

typedef std::map<std::string, KKSG::PlatNotice> CPlatNoticeInfoMap;
class CPlatNoticeMgr
{
	CPlatNoticeMgr();
	~CPlatNoticeMgr();

	DECLARE_SINGLETON(CPlatNoticeMgr)

public:
	bool Init();
	void Uninit();
	const CPlatNoticeInfoMap GetNoticeList() { return m_oNoticeList;}
	void UpdateNotice(KKSG::PlatNotice oData);

	KKSG::PlatNotice* GetNotice(KKSG::LoginType loginType, KKSG::PlatType platId);
	KKSG::PlatNotice* GetNotice(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID);


private:
	std::string GetKey(UINT32 uType, UINT32 uAreaID, UINT32 uPlatID);
	CPlatNoticeInfoMap m_oNoticeList;
};
#endif
