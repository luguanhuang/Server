#ifndef __XSKILLSCRIPTMGR_H__
#define __XSKILLSCRIPTMGR_H__

class XSkill;

using namespace std;

typedef bool (*pSkillScript)(XSkill*, float);

class XSkillScriptMgr
{
public:
	static pSkillScript GetScriptFunc(string classname, string funcname);
};

#endif	//__XSKILLSCRIPTMGR_H__