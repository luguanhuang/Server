#ifndef __PUSHCONFIG_H__
#define __PUSHCONFIG_H__

//���ļ��������������Ƿ�Ϊ���͵�����;
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
