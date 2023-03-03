#ifndef __PTCI2D_REGISTLOGINSERVER_H__
#define __PTCI2D_REGISTLOGINSERVER_H__

// generate by ProtoGen at date: 2015/10/12 10:50:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2D_REGISTLOGINSERVER_TYPE 42710

class PtcI2D_RegistLoginServer : public CProtocol
{
public:
    explicit PtcI2D_RegistLoginServer() : CProtocol(PTCI2D_REGISTLOGINSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2D_RegistLoginServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif