#include "pch.h"
#include "uidhelper.h"
#include "time.h"
#include "config.h"

union UID
{
	UINT64 id;
	struct {
		UINT16 index;
		UINT16 tag;	//line:3 serverid:13
		UINT32 timestamp;
	} _;
};

static UINT32 uidbegin = 1;

UINT64 UIDHelper::CreateUID(UIDType type)
{
	UID uid;
	uid._.timestamp = (UINT32)(time(NULL));
	uid._.index = uidbegin++;
	int high = 0;
	int low = 0;
	if (GSConfig::Instance()->IsCrossGS())
	{
		high = ((1 << 3) - 1);
		low = GSConfig::Instance()->GetLine();
	}
	else
	{
		high = GSConfig::Instance()->GetLine();
		low = GSConfig::Instance()->GetServerID();
	}
	uid._.tag = (high << 13) + low;
	return uid.id;
}

void UIDHelper::Test()
{
	for (int i = 0; i < 20; ++i)
	{
		UINT64 uid = UIDHelper::CreateUID(UID_Item);
		SSWarn<<"type:"<<UID_Item<<", uid:"<<uid<<END;
	}
	for (int i = 0; i < 20; ++i)
	{
		UINT64 uid = UIDHelper::CreateUID(UID_Shop);
		SSWarn<<"type:"<<UID_Shop<<", uid:"<<uid<<END;
	}
	for (int i = 0; i < 20; ++i)
	{
		UINT64 uid = UIDHelper::CreateUID(UID_Reward);
		SSWarn<<"type:"<<UID_Reward<<", uid:"<<uid<<END;
	}
	for (int i = 0; i < 20; ++i)
	{
		UINT64 uid = UIDHelper::CreateUID(UID_Pet);
		SSWarn<<"type:"<<UID_Pet<<", uid:"<<uid<<END;
	}
}

UINT32 UIDHelper::GetNonCrossIDServerID(UINT64 id)
{
	UID uid;
	uid.id = id;
	UINT16 tag = uid._.tag;
	UINT16 flag = 0x7;
	tag &= ~(flag << 13);
	return tag;
}

UINT32 UIDHelper::GetRoleIDServerID(UINT64 id)
{
	return GetLoWord(GetHiDword(id));
}
