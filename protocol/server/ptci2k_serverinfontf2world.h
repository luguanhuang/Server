#ifndef __PTCI2K_SERVERINFONTF2WORLD_H__
#define __PTCI2K_SERVERINFONTF2WORLD_H__

// generate by ProtoGen at date: 2017/2/5 14:07:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2K_SERVERINFONTF2WORLD_TYPE 46221

class PtcI2K_ServerInfoNtf2World : public CProtocol
{
public:
    explicit PtcI2K_ServerInfoNtf2World() : CProtocol(PTCI2K_SERVERINFONTF2WORLD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2K_ServerInfoNtf2World()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerInfoPara m_Data;
};

#endif