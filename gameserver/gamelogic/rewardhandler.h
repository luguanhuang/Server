#ifndef __REWARDHANDLER_H__
#define __REWARDHANDLER_H__

struct SRewardInfo;
class CRewardRecord;
class Role;
class CRewardHandler
{
public:
	CRewardHandler(UINT32 dwType);
	virtual ~CRewardHandler();
	
	///> �жϼ�¼���ڣ�����:���������¼��㽱����û����:������
	virtual bool Update(CRewardRecord* pRecord, UINT32 dwType) { return true; }
	///> ������ҽ���
	virtual bool GetReward(Role* pRole, SRewardInfo& stRewardInfo);
	///> ��ʱ���º�����
	bool OnTimer(UINT32 dwType);

protected:
	UINT32 m_dwType;
};

///> ��ͽ������������
class CDinnerRewardHandler : public CRewardHandler
{
public:
	CDinnerRewardHandler(UINT32 dwType);
	virtual ~CDinnerRewardHandler();

	virtual bool Update(CRewardRecord* pRecord, UINT32 dwType);
	virtual bool GetReward(Role* pRole, SRewardInfo& stRewardInfo);

private:
	UINT32 m_begin;
	UINT32 m_end;
	UINT32 m_crossday;
};

///> ��ͽ������������
class CSupperRewardHandler : public CRewardHandler
{
public:
	CSupperRewardHandler(UINT32 dwType);
	virtual ~CSupperRewardHandler();

	virtual bool Update(CRewardRecord* pRecord, UINT32 dwType);
	virtual bool GetReward(Role* pRole, SRewardInfo& stRewardInfo);

private:
	UINT32 m_begin;
	UINT32 m_end;
	UINT32 m_crossday;
};

#endif