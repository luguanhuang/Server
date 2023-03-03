#ifndef __PTCM2D_UPDATECUSTOMBATTLEONDB_H__
#define __PTCM2D_UPDATECUSTOMBATTLEONDB_H__

// generate by ProtoGen at date: 2017/5/6 9:44:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_UPDATECUSTOMBATTLEONDB_TYPE 34368

class PtcM2D_UpdateCustomBattleOnDB : public CProtocol
{
public:
    explicit PtcM2D_UpdateCustomBattleOnDB() : CProtocol(PTCM2D_UPDATECUSTOMBATTLEONDB_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_UpdateCustomBattleOnDB()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateCustomBattleOnDB m_Data;
};

#endif