#ifndef __PUSHCONFIG_H__
#define __PUSHCONFIG_H__

//本文件是用来管理本服是否为推送的配置;
class CPushConfigMgr
{
	CPushConfigMgr();
	~CPushConfigMgr();
	DECLARE_SINGLETON(CPushConfigMgr)

public:
	bool Init();
	void Uninit();
	bool GetIsWxPush(){ return m_bIsWxPush; }

private:
	bool ReadDataFromDB();
	bool m_bIsWxPush;
};
#endif
