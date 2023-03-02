#ifndef __TOKENVERIFYMGR_H__
#define __TOKENVERIFYMGR_H__

#include "WebFetchThread.h"
#include "SQLThread.h"
#include "pb/project.pb.h"
#include "gatemgr/gateinfo.h"

#define TOKEN_LEN 6	// [random:4-login:1-gm:1]


struct UToken
{
	unsigned int data[TOKEN_LEN];
	const char *str() { return (char*) data; }
	const int size() { return sizeof(data); }

	void Format(char *buf, int len) const
	{
		assert(len > TOKEN_LEN*8);
		unsigned char *pChar = (unsigned char*)data;
		const char HexDigit[] = "0123456789abcdef";
		for (int i = 0; i < TOKEN_LEN*4; ++i)
		{
			buf[2*i] =  HexDigit[(pChar[i] >> 4) & 0xF];
			buf[2*i+1] = HexDigit[pChar[i] & 0xF];
		}
		buf[TOKEN_LEN*8] = 0;
	}

	static bool FromString(UToken &uid, const char *data, int size)
	{
		if (size != uid.size())
		{
			return false;
		}

		memcpy(uid.data, data, size);
		return true;
	}
};

struct UTokenHash
{
	unsigned long operator()(const UToken &id) const
	{
		return (id.data[0] << 17) + (id.data[1] << 15) + (id.data[2] >> 13) + (id.data[3] >> 11);
	}
};

struct UTokenEqual
{
	bool operator()(const UToken &id, const UToken &id2) const
	{
		return id2.data[0] == id.data[0] &&
			   id2.data[1] == id.data[1] &&
			   id2.data[2] == id.data[2] &&
			   id2.data[3] == id.data[3] && 
			   id2.data[4] == id.data[4];
	}
};

// 给客户端返回用
struct UserGateInfo
{
	UserGateInfo():serverID(-1),isgm(false), isFreeFlow(false), ccType(CCType_Normal) {}
	UToken uid;
	std::string phone;
	UINT32 serverID;
	bool isgm;
	std::map<UINT32, UINT32> serverid2level;
	bool isFreeFlow;
	int ccType;
	std::vector<KKSG::PlatFriendServer> friendServer;
	std::vector<UINT32> bespeakServerIds;
};

// 内部token记录信息
struct UserInfo
{
	UserInfo(): isused(false), isgm(false), accountBackFlow(false) {}

	std::string account;
	time_t tokenExpireTime;
	bool isused;
	bool isgm;
	bool accountBackFlow;
};

#define SQLTHREAD_COUNT 5

class TokenVerifyMgr
{
	TokenVerifyMgr();
	~TokenVerifyMgr();
	DECLARE_SINGLETON(TokenVerifyMgr)

public:

	bool Init();
	void Uninit();

	UToken CreateUUID(bool isgm);
	UserInfo* FindUserInfo(const UToken &token);
	void AddToken(const UToken &token, const UserInfo& user);


	void AddPFVerifyTask(IWebFetchTask *pTask);
	void AddDBTask(ISQLTask *pTask);
	void Update();


	// 统计
	UINT32 GetHttpTaskNum() { return m_httpTaskNum; }
	void ClearHttpTaskNum() { m_httpTaskNum = 0; }
	UINT32 GetDbTaskNum() { return m_dbTaskNum; }
	void ClearDbTaskNum() { m_dbTaskNum = 0; }

private:
	int m_sqlRandIndex;
	WebFetchThreadPool m_pfThread;
	SQLThread m_dbThread[SQLTHREAD_COUNT];
	time_t m_NextCheckTime;
	UINT32 m_httpTaskNum;
	UINT32 m_dbTaskNum;

	std::unordered_map<UToken, UserInfo, UTokenHash, UTokenEqual> m_LoginTokenInfo;
};

#endif // __TOKENVERIFYMGR_H__