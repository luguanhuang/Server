#ifndef _GATE_INFO_H_
#define _GATE_INFO_H_

#include "pb/enum.pb.h"
#include "pb/project.pb.h"

struct GateIPAddr
{
	std::string ip;
	int port;
};


// ����������״̬�� ����ά�õ�
enum ServerSetState
{
	ServerSetState_Maintain = 0,
	ServerSetState_New = 1,
	ServerSetState_Smooth = 2,
	ServerSetState_Hot = 3,
	ServerSetState_Full = 4,
	ServerSetState_Recommend = 5,
	ServerSetState_Auto = 6,

	ServerSetState_Max,
};

enum CCType
{
	CCType_Normal = -1,
	CCType_LianTong = 0,
	CCType_DianXin = 1,
	CCType_YiDong = 2,
};


struct UserGateInfo;
// ע��: ��Ҫ��ָ��, ������
struct GateInfo
{
	GateInfo(): autoRecommend(false), backFlowLevel(0) {}

	int serverID;
	bool isOpen;
	bool autoRecommend;		// �Զ��Ƽ�
	UINT32 state;
	UINT32 setState;		// ���õ�״̬
	UINT32 backFlowLevel;	// �����ȼ�

	UINT32 registerAccount;
	UINT32 onlineRole;
	UINT32 openTime;			// ����ʱ��
	UINT32 fullRegisterTime;	// �״���ע��ʱ��

	UINT32 clientState;			// ���ͻ��˵�״̬
	UINT32 clientFlag;			// ���ͻ��˵ĽǱ�

	std::string serverName;
	std::string zoneName;
	std::vector<UINT32> channel;
	std::vector<GateIPAddr> Hosts;
	std::vector<GateIPAddr> FreeHosts;		// �������

	/*
	std::vector<GateIPAddr> LianTongHosts;	// ��ͨ�������
	std::vector<GateIPAddr> YiDongHosts;	// �ƶ��������
	std::vector<GateIPAddr> DianXinHosts;	// �����������
	*/

	void UpdateState();
	void UpdateClientState();

	void ParseChannel(const std::string& str);
	void ParseIpAddr(const std::string& str, std::vector<GateIPAddr>& hosts);

	void FillGateInfo(int ccType, bool isFreeFlow, KKSG::LoginGateData& data);

	bool IsBackFlowServer()
	{
		return backFlowLevel != 0;
	}

	bool HasChannel(KKSG::LoginType loginType)
	{
		return std::find(channel.begin(), channel.end(), loginType) != channel.end(); 
	}

	bool CanSee(KKSG::LoginType loginType, bool isInWhiteList)
	{
		if (std::find(channel.begin(), channel.end(), loginType) == channel.end())
		{
			return false;
		}
		if (isOpen == false && isInWhiteList == false)
		{
			return false;
		}
		return true;
	}

	bool IsRecommend()
	{
		return state == ServerSetState_Recommend;
	}

	GateIPAddr *GetRandomHost(int ccType, bool isFreeFlow);
};

#endif