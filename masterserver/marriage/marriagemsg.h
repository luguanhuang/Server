#ifndef __MARRIAGEMSG_H__
#define __MARRIAGEMSG_H__

#include "role/switch.h"
#include "pb/enum.pb.h"

class Marriage;

struct MarriageApplyInfo
{
	UINT32 applyTime;
	KKSG::WeddingType applyType;
};

class MarriageMsg
{
public:
	MarriageMsg(Marriage* pMarriage);
	~MarriageMsg();

	void Update();

	void AddApplyMsg(UINT64 roleID, KKSG::WeddingType type);

	bool CheckApplyInfoExist(UINT64 srcID, UINT64 destID, KKSG::WeddingType& type);
	void RemoveApplyInfo(UINT64 srcID, UINT64 destID);

private:
	Marriage* m_pMarriage;
	std::map<UINT64, MarriageApplyInfo> m_applyList;
};

#endif // 
