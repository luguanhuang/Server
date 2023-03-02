#ifndef __UIDHELPER_H__
#define __UIDHELPER_H__

enum UIDType
{
	UID_Item,
	UID_Shop,
	UID_Reward,
	UID_Pet,
	UID_Mail,

	UID_Common,	///> 位数不够，都用common吧

	UID_Max = ((1 << 3) - 1), 
};

class UIDHelper
{
public:
	static UINT64 CreateUID(UIDType type);

	static void Test();

	static UINT32 GetNonCrossIDServerID(UINT64 id);

	static UINT32 GetRoleIDServerID(UINT64 id);
};

#endif