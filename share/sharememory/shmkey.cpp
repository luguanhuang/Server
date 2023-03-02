#include "pch.h"
#include "sharememory/shmkey.h"
#include "config.h"

UINT32 GetShmKey(ShmType type, UINT32 offset)
{
	return Config::GetConfig()->GetServerID() * 1000 + type * 100 + offset;
}
