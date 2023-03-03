#ifndef __PTCM2C_CLOSESKYCRAFTELINTF_H__
#define __PTCM2C_CLOSESKYCRAFTELINTF_H__

// generate by ProtoGen at date: 2017/4/11 17:12:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_CLOSESKYCRAFTELINTF_TYPE 50236

class PtcM2C_CloseSkyCraftEliNtf : public CProtocol
{
public:
    explicit PtcM2C_CloseSkyCraftEliNtf() : CProtocol(PTCM2C_CLOSESKYCRAFTELINTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_CloseSkyCraftEliNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CloseSkyCraftEliNtf m_Data;
};

#endif