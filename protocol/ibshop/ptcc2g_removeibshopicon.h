#ifndef __PTCC2G_REMOVEIBSHOPICON_H__
#define __PTCC2G_REMOVEIBSHOPICON_H__

// generate by ProtoGen at date: 2016/9/7 21:20:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_REMOVEIBSHOPICON_TYPE 33988

class PtcC2G_RemoveIBShopIcon : public CProtocol
{
public:
    explicit PtcC2G_RemoveIBShopIcon() : CProtocol(PTCC2G_REMOVEIBSHOPICON_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_RemoveIBShopIcon()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RemoveIBShopIcon m_Data;
};

#endif