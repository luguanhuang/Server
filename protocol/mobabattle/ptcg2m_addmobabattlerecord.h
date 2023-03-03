#ifndef __PTCG2M_ADDMOBABATTLERECORD_H__
#define __PTCG2M_ADDMOBABATTLERECORD_H__

// generate by ProtoGen at date: 2017/6/18 15:54:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ADDMOBABATTLERECORD_TYPE 36411

class PtcG2M_AddMobaBattleRecord : public CProtocol
{
public:
    explicit PtcG2M_AddMobaBattleRecord() : CProtocol(PTCG2M_ADDMOBABATTLERECORD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_AddMobaBattleRecord()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddMobaBattleRecordData m_Data;
};

#endif