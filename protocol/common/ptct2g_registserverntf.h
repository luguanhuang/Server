#ifndef __PTCT2G_REGISTSERVERNTF_H__
#define __PTCT2G_REGISTSERVERNTF_H__

// generate by ProtoGen at date: 2017/10/10 16:01:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2G_REGISTSERVERNTF_TYPE 3070

class PtcT2G_RegistServerNtf : public CProtocol
{
public:
    explicit PtcT2G_RegistServerNtf() : CProtocol(PTCT2G_REGISTSERVERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2G_RegistServerNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif