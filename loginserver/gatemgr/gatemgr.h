#ifndef __GATEMGR_H__
#define __GATEMGR_H__

#include "cmdline.h"
#include "table/GateIPTable.h"
#include "pb/enum.pb.h"
#include "pb/project.pb.h"
#include "gateinfo.h"

struct UserGateInfo;
class GateMgr
{
	GateMgr();
	~GateMgr();
	DECLARE_SINGLETON(GateMgr)
public:
	bool Init();
	void Uninit();

	//bool LoadFromXml();

	void LoadFromDB();
	void ReloadFromDB();

	void ParseFrom(std::vector<GateInfo>& infos, bool reload);

	GateInfo *GetServerInfo(int serverID);

	int GetRecommendServerId(KKSG::LoginType loginType, bool isWhite);
	GateInfo* GetRecommendServerInfo(KKSG::LoginType loginType, bool isWhite);

	GateInfo* GetRecommendServerInfo(UserGateInfo* pUserInfo, KKSG::LoginType loginType, bool isInWhiteList);
	void FillServerGateInfo(KKSG::LoginType loginType, bool inWhiteList, KKSG::QueryGateRes& res, UserGateInfo* pUserInfo);
	void FillDummyRecommendServerInfo(KKSG::LoginType loginType, bool isWhite, KKSG::QueryGateRes& res, UserGateInfo* pUserInfo);
	void FillRecommendServerInfo(KKSG::LoginType loginType, bool isWhite, KKSG::QueryGateRes& res, UserGateInfo* pUserInfo);

	void ServerInfoNotifyToMs(UINT32 dwConnID = 0);
	void ServerInfoNotifyToWorld(UINT32 dwConnID = 0);
	void ServerInfoNotifyToIdip(KKSG::ServerInfoPara* pdata);

	std::map<int, GateInfo>& GetAllServer() { return m_gateInfos; }
private:
	void Clear();
	void DumpGate();
	void ParseRecommendServer(GateInfo& gateInfo);
	void CheckRecommendServer();

	void FillServerInfo(KKSG::ServerInfo& data, GateInfo& info);

	static bool CmdLineGate(const CmdLineArgs &args, std::string &outputMessage);

	class _RecommendInfo
	{
	public:
		_RecommendInfo(): serverId(0), sequence(0) {}
		inline int GetRecommend()
		{
			return serverId;
			/*
			if (serverIds.empty())
			{
				return 0;
			}
			return serverIds[(sequence++) % serverIds.size()];
			*/
		}

		inline void AddServerId(UINT32 id)
		{
			if (serverId <= 0)
			{
				serverId = id;
			}
			else if (id < serverId)
			{
				serverId = id;
			}
			serverIds.push_back(id);
			std::sort(serverIds.begin(), serverIds.end(), std::greater<int>());
		}

		inline bool HasRecommend()
		{
			return serverIds.empty() == false;
		}

		void Reset()
		{
			serverIds.clear();
			serverId = 0;
		}

		std::vector<int> serverIds;
		int serverId;
		UINT32 sequence;
	};

private:
	std::map<int, GateInfo> m_gateInfos;
	_RecommendInfo m_recommendServer[KKSG::LoginType_ARRAYSIZE];
};


#endif // __GATEMGR_H__