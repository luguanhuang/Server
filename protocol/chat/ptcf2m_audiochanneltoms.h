#ifndef __PTCF2M_AUDIOCHANNELTOMS_H__
#define __PTCF2M_AUDIOCHANNELTOMS_H__

// generate by ProtoGen at date: 2017/1/3 11:04:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCF2M_AUDIOCHANNELTOMS_TYPE 23192

class PtcF2M_AudioChannelToMs : public CProtocol
{
public:
    explicit PtcF2M_AudioChannelToMs() : CProtocol(PTCF2M_AUDIOCHANNELTOMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcF2M_AudioChannelToMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatInfo m_Data;
};

#endif