#ifndef __SECURITYMOBSTATISTICS_H__
#define __SECURITYMOBSTATISTICS_H__

#include "unit/combatattrdef.h"
#include "define.h"
#include <map>

struct HurtInfo;
struct ProjectDamageResult;
class Role;
class Unit;

class XSecurityMobStatistics
{
public:
	class XMobInfo
	{
	public:
		UINT32 _TemplateID;
		int _CastCount;
		float _AttackTotal;

		void Reset();

		void Merge(const XMobInfo& other);
	};

private:
	std::map<UINT32, XMobInfo*> _MobInfos;

private:
	XMobInfo* _TryGetMobInfo(UINT32 templateID);

public:

	~XSecurityMobStatistics();
	const std::map<UINT32, XMobInfo*>& GetMobInfos() { return _MobInfos;}

	void OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void OnCast(UINT32 skillID);

	void Reset();

	void Merge(const XSecurityMobStatistics& other);
	void Append(Unit* pUnit);

	static XSecurityMobStatistics* TryGetStatistics(Unit *pUnit);
};

#endif