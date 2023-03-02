#include "pch.h"
#include "shmmgr.h"
#include "sharememory/shmkey.h"
#include "cmdline.h"
#include "config.h"

static bool ShmStat(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() == 1)
	{
		SSWarn << "SceneShm Stat: " << ShmMgr::Instance()->GetSceneShm().GetStatStr() << END;
		SSWarn << "AccountShm Stat: " << ShmMgr::Instance()->GetAccountShm().GetStatStr() << END;
		SSWarn << "RoleShm Stat: " << ShmMgr::Instance()->GetRoleShm().GetStatStr() << END;
		return true;
	}

	if (args[1] == "scene")
	{
		SSWarn << "SceneShm Stat: " << ShmMgr::Instance()->GetSceneShm().GetStatStr() << END;
	}
	else if (args[1] == "account")
	{
		SSWarn << "AccountShm Stat: " << ShmMgr::Instance()->GetAccountShm().GetStatStr() << END;
	}
	else if (args[1] == "role")
	{
		SSWarn << "RoleShm Stat: " << ShmMgr::Instance()->GetRoleShm().GetStatStr() << END;
	}
	else
	{
		LogWarn("no shm type: %s", args[1].c_str());
	}
	return true;
}

bool ShmMgr::Init()
{
	CmdLine::RegistCmd("shmstat", ShmStat);
	UINT64 key = GetShmKey(ShmType_NS_SceneManager, ControlConfig::Instance()->GetShmKeyOffset("nsscene"));
	if (m_shmScene.Init(key, ControlConfig::Instance()->GetChunkNum("scene", 50000)) == false)
	{
		LogError("init scene shm failed");
		return false;
	}
	key = GetShmKey(ShmType_NS_RoleManager, ControlConfig::Instance()->GetShmKeyOffset("nsrole"));
	if (m_shmRole.Init(key, ControlConfig::Instance()->GetChunkNum("role", 10000)) == false)
	{
		LogError("init role shm failed");
		return false;
	}
	key = GetShmKey(ShmType_NS_Account, ControlConfig::Instance()->GetShmKeyOffset("nsaccount"));
	if (m_shmAccount.Init(key, ControlConfig::Instance()->GetChunkNum("account", 20000)) == false)
	{
		LogError("init account shm failed");
		return false;
	}

	return true;
}


void ShmMgr::Uninit()
{
	m_shmScene.Uninit();
	m_shmRole.Uninit();
	m_shmAccount.Uninit();
}
