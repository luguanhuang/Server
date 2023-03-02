#include "pch.h"
#include "common/rpcm2f_registerms2fm.h"
#include "network/fmlink.h"
#include "chat/fmmgr.h"
#include "pb/project.pb.h"

// generate by ProtoGen at date: 2017/2/3 21:57:51

RPC_CLIENT_IMPLEMETION(RpcM2F_RegisterMs2Fm, RegisterMs2FmArg, RegisterMs2FmRes)

void RpcM2F_RegisterMs2Fm::OnReply(const RegisterMs2FmArg &roArg, const RegisterMs2FmRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Register To fm Success, serverId[%u] is_master[%u]", roRes.fmsvrid(), roRes.is_master());
		//最恶劣的情况，主的一直没起来，从头到尾都没办法起来
		if (roRes.whitelist().role_size()!=0)
		{
			FMMgr::Instance()->ClearWhiteList();
			for(auto i = 0; i< roRes.whitelist().role_size(); i++)
			{
				const KKSG::FMWhiteRoleData*  pData = &(roRes.whitelist().role(i));
				FMMgr::Instance()->AddWhiteList(pData->acc(),pData->nick());
				LogDebug("---RpcM2F_RegisterMs2Fm--- acc=%s  nick=%s",pData->acc().c_str(),pData->nick().c_str());
			}
		}

		if (roRes.is_master())
		{
			FMLink::Instance()->SetMasterFmConnId(m_dwConnID);
			if (roRes.radiolist().roleid_size()!=0)
			{
				FMMgr::Instance()->ClearAudio();
				for (UINT32 i = 0; i < roRes.radiolist().name_size(); i++)
				{
					std::string name = roRes.radiolist().name(i);
					LogDebug("--name=%s roleid=%llu--", roRes.radiolist().name(i).c_str(),roRes.radiolist().roleid(i));
					FMMgr::Instance()->AddAudio(name,roRes.radiolist().roleid(i));
				}
			}
		}
	}
	else
	{
		LogError("Register to fm server [%u] failed, error code [%d]", roRes.fmsvrid(), roRes.result());
	}
}

void RpcM2F_RegisterMs2Fm::OnTimeout(const RegisterMs2FmArg &roArg, const CUserData &roUserData)
{
	LogError("Register To Fm Timeout");
}
