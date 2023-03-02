#ifndef __FRIENDRECORD_H__
#define __FRIENDRECORD_H__

namespace KKSG
{
	class FriendRecord;
}

class CFriendRecord
{
public:
	CFriendRecord();

	bool Load(const KKSG::FriendRecord& record);
	void Save(KKSG::FriendRecord& record);

	void Reset();
	void Update(); 

	bool IsChanged() { return m_ischanged; }
	void SetChanged(bool ischange) { m_ischanged = ischange; }

	bool IsReceiveLimit();
	bool IsSendLimit();

	void AddReceive();
	bool AddSend(UINT64 roleid);

	UINT32 GetReceiveLeft();
	UINT32 GetSendLeft();

	bool UpdateQueryTime();

	void UpdateRandom(time_t nowtime); 
	void AddRandom(time_t nowtime, UINT64 id);
	bool IsRandomValid(time_t nowtime, UINT64 id);

private:
	bool m_ischanged;
	UINT32 m_uniqueday;
	UINT32 m_receive;
	UINT32 m_send;
	UINT32 m_lastquerytime;
	std::set<UINT64> m_sends;

	std::map<UINT64, time_t> m_randomfriends;
};

#endif