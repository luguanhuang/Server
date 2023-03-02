#ifndef __PROTOSTATISTICS_H__
#define __PROTOSTATISTICS_H__

#include <unordered_map>

struct ProtoDetail
{
	UINT64 SendBytes;
	UINT64 RecvBytes;
	UINT64 ProcessTime;
	UINT32 SendCount;
	UINT32 RecvCount;

	UINT64 SendBytesInPeriod;
	UINT64 RecvBytesInPeriod;
	UINT32 SendCountInPeriod;
	UINT32 RecvCountInPeriod;

	UINT64 MaxSendBytesInPeriod;
	UINT64 MaxRecvBytesInPeriod;
	UINT32 MaxSendCountInPeriod;
	UINT32 MaxRecvCountInPeriod;


	ProtoDetail()
		:SendBytes(0)
		,RecvBytes(0)
		,ProcessTime(0)
		,SendCount(0)
		,RecvCount(0)
		,SendBytesInPeriod(0)
		,RecvBytesInPeriod(0)
		,SendCountInPeriod(0)
		,RecvCountInPeriod(0)
		,MaxSendBytesInPeriod(0)
		,MaxRecvBytesInPeriod(0)
		,MaxSendCountInPeriod(0)
		,MaxRecvCountInPeriod(0)
	{
	}
};

class ProtoStatistics
{
public:
	ProtoStatistics();
	~ProtoStatistics();

	static ProtoStatistics *Instance();
	static void Destory();

	void Reset();
	void SetEnable(bool flag);
	void AddProtoRecvBytes(UINT32 proto, UINT32 bytes, UINT64 ProcessTime);
	void AddProtoSendBytes(UINT32 proto, UINT32 bytes, UINT64 ProcessTime);
	UINT64 GetProtoRecvBytes(UINT32 proto);
	UINT64 GetProtoSendBytes(UINT32 proto);
	ProtoDetail *GetProtoDetail(UINT32 proto);

	typedef std::unordered_map<UINT32, ProtoDetail>::iterator iterator;
	iterator begin() { return m_ProtoDetails.begin(); }
	iterator end() { return m_ProtoDetails.end(); }

	std::string DebugInfo();
	std::string HtmlInfo();

	bool IsEnable() const { return m_Enable; }
private:
	bool m_Enable;
	std::unordered_map<UINT32, ProtoDetail> m_ProtoDetails;
};

#endif // __PROTOSTATISTICS_H__