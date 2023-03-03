#ifndef __PTCF2M_FMMASTERINFONTF_H__
#define __PTCF2M_FMMASTERINFONTF_H__

// generate by ProtoGen at date: 2017/3/11 11:58:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCF2M_FMMASTERINFONTF_TYPE 25260

class PtcF2M_FmMasterInfoNtf : public CProtocol
{
public:
    explicit PtcF2M_FmMasterInfoNtf() : CProtocol(PTCF2M_FMMASTERINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcF2M_FmMasterInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FmMasterInfo m_Data;
};

#endif