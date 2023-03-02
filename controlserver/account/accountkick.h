#ifndef _H_Account_Kick_H__
#define _H_Account_Kick_H__

#include "commondef.h"


class IAccountKickListener
{
public:
	virtual ~IAccountKickListener() {}
	virtual void OnKickReply(KKSG::ErrorCode nErrCode, const std::string& strAccount, UINT64 qwRoleID) = 0;
};


struct AccountKickInfo 
{
	UINT64			m_qwRoleID;
	KKSG::KickType	m_nKickType;
	UINT32			m_dwRpcDelayed;

	AccountKickInfo()
		:m_qwRoleID(INVALID_ROLE_ID)
		,m_nKickType(KKSG::KICK_NORMAL)
		,m_dwRpcDelayed(0)
	{
	}
};


//
//Used for idip
//
class CAccountKicker
{
	CAccountKicker();
	virtual ~CAccountKicker();
	DECLARE_SINGLETON(CAccountKicker)
public:
	bool Init();
	void Uninit();

	KKSG::ErrorCode KickAccount(UINT64 qwRoleID, KKSG::KickType nKickType, AccountKickInfo** ppKickInfo);
	void OnKickReply(const std::string& strAccount, KKSG::ErrorCode nErrCode, KKSG::KickType& nKickType);
private:
	typedef std::unordered_map<std::string, AccountKickInfo>	CKickInfoMap;
	CKickInfoMap	m_oMap;
};

#endif