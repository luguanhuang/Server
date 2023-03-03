#ifndef __PTCM2C_GUILDBONUSGETALL_H__
#define __PTCM2C_GUILDBONUSGETALL_H__

// generate by ProtoGen at date: 2016/12/24 11:49:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDBONUSGETALL_TYPE 55177

class PtcM2C_GuildBonusGetAll : public CProtocol
{
public:
    explicit PtcM2C_GuildBonusGetAll() : CProtocol(PTCM2C_GUILDBONUSGETALL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildBonusGetAll()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBonusGetAllData m_Data;
};

#endif