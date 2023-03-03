#ifndef __PTCF2M_FMWHITELISTRELOADNTF_H__
#define __PTCF2M_FMWHITELISTRELOADNTF_H__

// generate by ProtoGen at date: 2017/2/15 21:28:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCF2M_FMWHITELISTRELOADNTF_TYPE 25576

class PtcF2M_FMWhiteListReloadNtf : public CProtocol
{
public:
    explicit PtcF2M_FMWhiteListReloadNtf() : CProtocol(PTCF2M_FMWHITELISTRELOADNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcF2M_FMWhiteListReloadNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FmReloadInfo m_Data;
};

#endif