#ifndef __PTCM2C_MODIFYGUILDNAMENTF_H__
#define __PTCM2C_MODIFYGUILDNAMENTF_H__

// generate by ProtoGen at date: 2017/4/10 11:28:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MODIFYGUILDNAMENTF_TYPE 18518

class PtcM2C_ModifyGuildNameNtf : public CProtocol
{
public:
    explicit PtcM2C_ModifyGuildNameNtf() : CProtocol(PTCM2C_MODIFYGUILDNAMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ModifyGuildNameNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ModifyArg m_Data;
};

#endif