#ifndef __ANTI_ADDICTIONMGR_H__
#define __ANTI_ADDICTIONMGR_H__
#include "unit/role.h"
#include "util/jsonutil.h"
#include "appid.h"

#define AntiAddictionCheckTime 300 //5分钟检查一下;
#define AntiAddictionRemindTime 3*3600 //单次在线提醒时长;
#define MAX_POST_COUNT 128	//最大上报个数

enum ReportType
{
	ReportTypeSingle = 1,//单次在线3小时提醒;
	ReportTypeTotal = 2,//当天累计在线8小时;
};

enum HealthyGameFlag
{
	HealthyGameFlag0 = 0,//不需要处理
	HealthyGameFlag1 = 1,//休息提醒
	HealthyGameFlag2 = 2,//强制下线休息
	HealthyGameFlag3 = 3,//宵禁
	HealthyGameFlag4 = 4,//禁玩
	HealthyGameFlag5 = 5,//单次强制下线提醒
};

enum HgReportType
{
	HgReportType1 = 1,//单次时长提醒；
	HgReportType2 = 2,//累计时长提醒;
	HgReportType3 = 3,//单次强制休息；
	HgReportType4 = 4,//累计强制休息；
	HgReportType5 = 5,//玩家宵禁；
	HgReportType6 = 6,//玩家禁玩
};

enum AntiAddictionMsgType
{
	MSG_TYPE_GET_CONF = 1,				//拉取游戏配置信息;
	MSG_TYPE_GET_USERINFO_SINGLE = 2,	//查询用户防沉迷信息;
	MSG_TYPE_UPDATE_USERINFO_SINGLE = 3,//更新用户防沉迷时长信息  ;  
	MSG_TYPE_GET_USERINFO_BATCH = 4,	//查询用户防沉迷信息--批量接口;
	MSG_TYPE_UPDATE_USERINFO_BATCH = 5,	//更新用户防沉迷时长信息--批量接口;
	MSG_TYPE_PUSH_EDNGAME = 6,			//上报用户退出游戏信息;
	MSG_TYPE_PUSH_ENDGAME_BATCH = 7,	//上报用户退出游戏信息--批量接口;
	MSG_TYPE_REPORT_REMINDED_BATCH = 8, //上报用户弹窗提醒信息--批量接口;
};

enum AntiAddictionAdultType
{
	AdultType0 = 0, //未成年
	AdultType1 = 1, //成年人
	AdultType2 = 2, //无身份资料
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

	std::vector<UINT32> m_ChildOnceGameRestTimeList;	//未成年人单次-休息提醒时间点列表（从小到大排列），单位为秒
	UINT32	m_ChildOnceGameForceExitTime;	//未成年人单次-强制下线时间点，单位为秒
	UINT32	m_ChildOnceGameForceRestTime;	//未成年人单次-强制下线休息时长，单位为秒
	std::vector<UINT32>	m_AdultOnceGameRestTimeList;	//成年人单次-休息提醒时间点列表（从小到大排列），单位为秒
	UINT32	m_AdultOnceGameForceExitTime;	//成年人单次-强制下线时间点，单位为秒
	UINT32	m_AdultOnceGameForceRestTime;	//成年人单次-强制下线休息时长，单位为秒
};
#endif