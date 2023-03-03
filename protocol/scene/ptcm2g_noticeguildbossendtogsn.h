#ifndef __PTCM2G_NOTICEGUILDBOSSENDTOGSN_H__
#define __PTCM2G_NOTICEGUILDBOSSENDTOGSN_H__

// generate by ProtoGen at date: 2016/7/29 20:34:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTICEGUILDBOSSENDTOGSN_TYPE 7857

class PtcM2G_noticeguildbossendtogsn : public CProtocol
{
public:
    explicit PtcM2G_noticeguildbossendtogsn() : CProtocol(PTCM2G_NOTICEGUILDBOSSENDTOGSN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_noticeguildbossendtogsn()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::sceneid m_Data;
};

#endif