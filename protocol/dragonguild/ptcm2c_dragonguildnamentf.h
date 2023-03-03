#ifndef __PTCM2C_DRAGONGUILDNAMENTF_H__
#define __PTCM2C_DRAGONGUILDNAMENTF_H__

// generate by ProtoGen at date: 2017/9/8 11:45:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_DRAGONGUILDNAMENTF_TYPE 35553

class PtcM2C_DragonGuildNameNtf : public CProtocol
{
public:
    explicit PtcM2C_DragonGuildNameNtf() : CProtocol(PTCM2C_DRAGONGUILDNAMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_DragonGuildNameNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DragonGuildNameNtf m_Data;
};

#endif