#ifndef __PTCN2R_NSCLOSENTF_H__
#define __PTCN2R_NSCLOSENTF_H__

// generate by ProtoGen at date: 2016/11/28 19:00:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2R_NSCLOSENTF_TYPE 44832

class PtcN2R_NsCloseNtf : public CProtocol
{
public:
    explicit PtcN2R_NsCloseNtf() : CProtocol(PTCN2R_NSCLOSENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2R_NsCloseNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NsCloseNtf m_Data;
};

#endif