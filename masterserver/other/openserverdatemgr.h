#ifndef __OPENSERVERDATE_MGR_H__
#define __OPENSERVERDATE_MGR_H__


class COpenServerDateMgr
{
	COpenServerDateMgr();
	~COpenServerDateMgr();
	DECLARE_SINGLETON(COpenServerDateMgr)

public:
	bool Init();
	void Uninit();

private:
	bool ReadDataFromDB();
};

#endif

