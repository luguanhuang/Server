#ifndef __PTCM2F_AUDIOCHANNELTOFM_H__
#define __PTCM2F_AUDIOCHANNELTOFM_H__

// generate by ProtoGen at date: 2017/1/3 11:07:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_AUDIOCHANNELTOFM_TYPE 32348

class PtcM2F_AudioChannelToFm : public CProtocol
{
public:
    explicit PtcM2F_AudioChannelToFm() : CProtocol(PTCM2F_AUDIOCHANNELTOFM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_AudioChannelToFm()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatInfo m_Data;
};

#endif