#ifndef __CUSTOMBATLLELIMIT_H__
#define __CUSTOMBATLLELIMIT_H__

struct LimitRecord
{
	time_t timestamp;
	UINT32 count;

	LimitRecord()
	{
		timestamp = 0;
		count = 0;
	}
};

class MatchLimit
{
public:
	MatchLimit();
	~MatchLimit();

	void SetParam(int count, int cd, int point);

	void Update();

	void Push(UINT64 roleida, int pointa, UINT64 roleidb, int pointb);
	bool IsOne2OneLimit(UINT64 roleida, int pointa, UINT64 roleidb, int pointb);
	bool IsLimit(UINT64 roleid, UINT64 other);

	bool IsNeedCheck(int point);

private:
	void InnerPush(UINT64 roleid, UINT64 other);

private:
	UINT32 limitcount;
	UINT32 limitcd;
	int limitpoint;
	std::unordered_map<UINT64, std::map<UINT64, LimitRecord>> m_roleid2limit;
};

#endif