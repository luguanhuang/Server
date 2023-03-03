#ifndef __PTCM2F_FMSHOWNTF_H__
#define __PTCM2F_FMSHOWNTF_H__

// generate by ProtoGen at date: 2016/11/30 9:58:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_FMSHOWNTF_TYPE 29146

class PtcM2F_FMShowNtf : public CProtocol
{
public:
    explicit PtcM2F_FMShowNtf() : CProtocol(PTCM2F_FMSHOWNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_FMShowNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMShow m_Data;
};

#endif