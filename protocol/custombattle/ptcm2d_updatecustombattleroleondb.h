#ifndef __PTCM2D_UPDATECUSTOMBATTLEROLEONDB_H__
#define __PTCM2D_UPDATECUSTOMBATTLEROLEONDB_H__

// generate by ProtoGen at date: 2017/5/6 9:45:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_UPDATECUSTOMBATTLEROLEONDB_TYPE 56056

class PtcM2D_UpdateCustomBattleRoleOnDB : public CProtocol
{
public:
    explicit PtcM2D_UpdateCustomBattleRoleOnDB() : CProtocol(PTCM2D_UPDATECUSTOMBATTLEROLEONDB_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_UpdateCustomBattleRoleOnDB()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateCustomBattleRoleOnDB m_Data;
};

#endif