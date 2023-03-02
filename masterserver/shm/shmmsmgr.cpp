#include "pch.h"
#include "shmmsmgr.h"
#include "sharememory/shmkey.h"
#include "cmdline.h"
#include "config.h"

static bool ShmMsStat(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() == 1)
	{
		SSWarn << "SceneShm Stat: " << ShmMsMgr::Instance()->GetFmRoleShm().GetStatStr() << END;
		return true;
	}
	
	if (args[1] == "rolefm")
	{
		SSWarn << "RoleShm Stat: " << ShmMsMgr::Instance()->GetFmRoleShm().GetStatStr() << END;
	}
	else
	{
		LogWarn("no shm type: %s", args[1].c_str());
	}
	return true;
}

bool ShmMsMgr::Init()
{
	CmdLine::RegistCmd("shmstat", ShmMsStat);
	UINT64 key = GetShmKey(ShmType_MS_FmRole, MSConfig::Instance()->GetShmKeyOffset("msfm"));
	if (m_shmFmRole.Init(GetShmKey(ShmType_MS_FmRole), 20000) == false)
	{
		LogError("init role fmshm failed");
		return false;
	}
	return true;
}


void ShmMsMgr::Uninit()
{
	m_shmFmRole.Uninit();
}
