#ifndef __ANTICHEAT_H__
#define __ANTICHEAT_H__

namespace KKSG
{
	class CliAntiCheatInfo;
}
class Role;


class AntiCheat
{
public:

	static bool IsCheat(Role* poRole, UINT32 dwSceneTmplID, UINT32 dwTimeBegin, const KKSG::CliAntiCheatInfo& roInfo);

private:
};

#endif // __ANTICHEAT_H__