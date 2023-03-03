#ifndef __PTCG2M_ADDGUILDBOSSATTR_H__
#define __PTCG2M_ADDGUILDBOSSATTR_H__

// generate by ProtoGen at date: 2016/8/3 22:29:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ADDGUILDBOSSATTR_TYPE 57234

class PtcG2M_AddGuildBossAttr : public CProtocol
{
public:
    explicit PtcG2M_AddGuildBossAttr() : CProtocol(PTCG2M_ADDGUILDBOSSATTR_TYPE)
    {
		
    }

    virtual ~PtcG2M_AddGuildBossAttr()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif