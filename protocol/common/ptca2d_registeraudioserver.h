#ifndef __PTCA2D_REGISTERAUDIOSERVER_H__
#define __PTCA2D_REGISTERAUDIOSERVER_H__

// generate by ProtoGen at date: 2016/6/22 14:24:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCA2D_REGISTERAUDIOSERVER_TYPE 9153

class PtcA2D_RegisterAudioServer : public CProtocol
{
public:
    explicit PtcA2D_RegisterAudioServer() : CProtocol(PTCA2D_REGISTERAUDIOSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcA2D_RegisterAudioServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif