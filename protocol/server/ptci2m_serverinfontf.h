#ifndef __PTCI2M_SERVERINFONTF_H__
#define __PTCI2M_SERVERINFONTF_H__

// generate by ProtoGen at date: 2017/1/12 20:19:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2M_SERVERINFONTF_TYPE 37845

class PtcI2M_ServerInfoNtf : public CProtocol
{
public:
    explicit PtcI2M_ServerInfoNtf() : CProtocol(PTCI2M_SERVERINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2M_ServerInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerInfoPara m_Data;
};

#endif