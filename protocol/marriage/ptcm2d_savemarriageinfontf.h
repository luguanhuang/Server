#ifndef __PTCM2D_SAVEMARRIAGEINFONTF_H__
#define __PTCM2D_SAVEMARRIAGEINFONTF_H__

// generate by ProtoGen at date: 2017/7/25 16:00:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEMARRIAGEINFONTF_TYPE 3951

class PtcM2D_SaveMarriageInfoNtf : public CProtocol
{
public:
    explicit PtcM2D_SaveMarriageInfoNtf() : CProtocol(PTCM2D_SAVEMARRIAGEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveMarriageInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SaveMarriage2DbData m_Data;
};

#endif