#ifndef __PTCM2C_RESWARTIMENTF_H__
#define __PTCM2C_RESWARTIMENTF_H__

// generate by ProtoGen at date: 2016/11/27 19:46:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARTIMENTF_TYPE 36825

class PtcM2C_ResWarTimeNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarTimeNtf() : CProtocol(PTCM2C_RESWARTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarTime m_Data;
};

#endif