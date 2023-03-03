#ifndef __PTCI2M_DELBANACCOUNTNTF_H__
#define __PTCI2M_DELBANACCOUNTNTF_H__

// generate by ProtoGen at date: 2017/4/11 9:35:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2M_DELBANACCOUNTNTF_TYPE 9091

class PtcI2M_DelBanAccountNtf : public CProtocol
{
public:
    explicit PtcI2M_DelBanAccountNtf() : CProtocol(PTCI2M_DELBANACCOUNTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2M_DelBanAccountNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BanAccontSimple m_Data;
};

#endif