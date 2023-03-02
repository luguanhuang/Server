#ifndef _H_Protocol_Stat_H__
#define _H_Protocol_Stat_H__


class CProtocolStat
{
	CProtocolStat();
	~CProtocolStat();
	DECLARE_SINGLETON(CProtocolStat)
public:
	bool Init();
	void Uninit();

	void OnRecv(UINT64 qwSessionID, UINT32 dwType, UINT32 dwLen);
	void OnReply(UINT64 qwSessionID, UINT32 dwType);

private:
	struct CStatInfo
	{
		UINT64	m_qwRecvTime;
		UINT64	m_qwReplyTime;
		CStatInfo()
			:m_qwRecvTime(0)
			,m_qwReplyTime(0)
		{
		}
	};

	typedef std::map<UINT64, CStatInfo>	CStatInfoMap;
	CStatInfoMap	m_oStatMap;
};

#endif