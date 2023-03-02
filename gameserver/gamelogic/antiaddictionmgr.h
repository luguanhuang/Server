#ifndef __ANTI_ADDICTIONMGR_H__
#define __ANTI_ADDICTIONMGR_H__
#include "unit/role.h"
#include "util/jsonutil.h"
#include "appid.h"

#define AntiAddictionCheckTime 300 //5���Ӽ��һ��;
#define AntiAddictionRemindTime 3*3600 //������������ʱ��;
#define MAX_POST_COUNT 128	//����ϱ�����

enum ReportType
{
	ReportTypeSingle = 1,//��������3Сʱ����;
	ReportTypeTotal = 2,//�����ۼ�����8Сʱ;
};

enum HealthyGameFlag
{
	HealthyGameFlag0 = 0,//����Ҫ����
	HealthyGameFlag1 = 1,//��Ϣ����
	HealthyGameFlag2 = 2,//ǿ��������Ϣ
	HealthyGameFlag3 = 3,//����
	HealthyGameFlag4 = 4,//����
	HealthyGameFlag5 = 5,//����ǿ����������
};

enum HgReportType
{
	HgReportType1 = 1,//����ʱ�����ѣ�
	HgReportType2 = 2,//�ۼ�ʱ������;
	HgReportType3 = 3,//����ǿ����Ϣ��
	HgReportType4 = 4,//�ۼ�ǿ����Ϣ��
	HgReportType5 = 5,//���������
	HgReportType6 = 6,//��ҽ���
};

enum AntiAddictionMsgType
{
	MSG_TYPE_GET_CONF = 1,				//��ȡ��Ϸ������Ϣ;
	MSG_TYPE_GET_USERINFO_SINGLE = 2,	//��ѯ�û���������Ϣ;
	MSG_TYPE_UPDATE_USERINFO_SINGLE = 3,//�����û�������ʱ����Ϣ  ;  
	MSG_TYPE_GET_USERINFO_BATCH = 4,	//��ѯ�û���������Ϣ--�����ӿ�;
	MSG_TYPE_UPDATE_USERINFO_BATCH = 5,	//�����û�������ʱ����Ϣ--�����ӿ�;
	MSG_TYPE_PUSH_EDNGAME = 6,			//�ϱ��û��˳���Ϸ��Ϣ;
	MSG_TYPE_PUSH_ENDGAME_BATCH = 7,	//�ϱ��û��˳���Ϸ��Ϣ--�����ӿ�;
	MSG_TYPE_REPORT_REMINDED_BATCH = 8, //�ϱ��û�����������Ϣ--�����ӿ�;
};

enum AntiAddictionAdultType
{
	AdultType0 = 0, //δ����
	AdultType1 = 1, //������
	AdultType2 = 2, //���������
};

struct stAntiAddictionComMsg
{
	INT32 m_iSeqID;
	INT32 m_iMsgType;
	INT32 m_iPlatID;
	INT32 m_iArea;
	std::string version;
	std::string appid;
	stAntiAddictionComMsg()
	{
		m_iSeqID = 0;
		m_iMsgType = 0;
		m_iPlatID = 0;
		m_iArea = 0;
		version = string("1.0");
		appid = string(GSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	}
};
class CAntiAddictionMgr : public ITimer
{
	CAntiAddictionMgr();
	~CAntiAddictionMgr();
	DECLARE_SINGLETON(CAntiAddictionMgr)

public:
	bool Init();
	void Uninit();
	void OnTimer(UINT32 dwID, UINT32 dwCount);
	void LoginOut(Role* role);
	void UpdateUserInfo(std::vector<Role*>& list);
	void UpdateRemind(std::vector<Role*>& list);
	void OnLogin(Role* role);
	void GetConf();
	std::string FillPostData(INT32 iPlatId, UINT32 serverid, INT32 iMsgType, rapidjson::Value& oBody , JsonBuilder& oBuilder);
	void CheckRetryGetConf();
	void SetIsGetConf(bool isgetconf=true) { m_IsGetConf = isgetconf; }

    void KickAccountNtfClientMsg(UINT64 roleidBeKicked, KKSG::KickType kt, UINT32 endTime, const std::string& strMsg, UINT32 hgflag);
	void SetConf(std::vector<UINT32>& ChildList, UINT32 ChildExitTime, UINT32 ChildRestTime, std::vector<UINT32>& AdultTimeList, UINT32 AdultExitTime, UINT32 AdultRestTime);
	UINT32 GetRemindTime(Role* role);
	UINT32 GetConfRemindCount(Role* role);
	UINT32 GetRestTime(Role* role);
	UINT32 GetForceExitTime(Role* role);
	std::string GetRemindMsg(Role* role, UINT32 iTime, INT32 type);
	void ReportRemind(Role* role, INT32 reporttype);
	void ReportRemindByHgFlag(Role* role, UINT32 hgflag);
	std::string GetForceExitMsg(Role* role, INT32 type, UINT32 nextLoginTime, UINT32 gameTime=0, UINT32 restTime=0);
private:
	bool m_IsRetryGetConf;
	bool m_IsGetConf;
	INT32 m_iSeqID;
	HTIMER m_TimeHandler;

	std::vector<UINT32> m_ChildOnceGameRestTimeList;	//δ�����˵���-��Ϣ����ʱ����б���С�������У�����λΪ��
	UINT32	m_ChildOnceGameForceExitTime;	//δ�����˵���-ǿ������ʱ��㣬��λΪ��
	UINT32	m_ChildOnceGameForceRestTime;	//δ�����˵���-ǿ��������Ϣʱ������λΪ��
	std::vector<UINT32>	m_AdultOnceGameRestTimeList;	//�����˵���-��Ϣ����ʱ����б���С�������У�����λΪ��
	UINT32	m_AdultOnceGameForceExitTime;	//�����˵���-ǿ������ʱ��㣬��λΪ��
	UINT32	m_AdultOnceGameForceRestTime;	//�����˵���-ǿ��������Ϣʱ������λΪ��
};
#endif