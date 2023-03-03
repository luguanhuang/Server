#ifndef __PTCM2C_RESWARRANKSIMPLEINFONTF_H__
#define __PTCM2C_RESWARRANKSIMPLEINFONTF_H__

// generate by ProtoGen at date: 2016/11/11 20:23:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARRANKSIMPLEINFONTF_TYPE 29973

class PtcM2C_ResWarRankSimpleInfoNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarRankSimpleInfoNtf() : CProtocol(PTCM2C_RESWARRANKSIMPLEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarRankSimpleInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarRankSimpleInfo m_Data;
};

#endif