#ifndef __NPCFEELINGMGR_H__
#define __NPCFEELINGMGR_H__

#include "util/utilsingleton.h"
#include "timer.h"

class CNpcFeelingMgr : public Singleton<CNpcFeelingMgr>, public ITimer
{
public:
	CNpcFeelingMgr();
	~CNpcFeelingMgr();

	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool ReadData();
	void WriteData();
	void SyncData(UINT32 gsLine = 0);

	void UpdateTopAvExp(const KKSG::NpcFlRoleExp& roleExp);

	void FillNpc2Role(KKSG::NpcFlRes& roRes);
	void UpdateNpc2Role(const KKSG::NpcFlNpc2Role& npc2role);

	void DayZero();
	void WeekZero();

	void Clear();

	void InfoPrint();
	void InfoPrint(const KKSG::NpcFlGlobal& data);

	void DebugMergeData(KKSG::NpcFlGlobal& data);
	void DebugAmendData(KKSG::NpcFlGlobal& data);

private:
	void AmendData();
	bool AmendData(::google::protobuf::RepeatedPtrField< ::KKSG::NpcFlRoleExp >& data);
	bool AmendData(::google::protobuf::RepeatedPtrField< ::KKSG::NpcFlNpc2Role >& data);

private:
	HTIMER m_handler;

	KKSG::NpcFlGlobal m_data;//内部数据做变动需要考虑合服
	bool m_amendFlag;

};

#endif