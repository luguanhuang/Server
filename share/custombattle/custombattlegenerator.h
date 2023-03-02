#ifndef __CUSTOMBATTLEGENERATOR_H__
#define __CUSTOMBATTLEGENERATOR_H__

#include "custombattledef.h"

class CustomBattle;
class CustomBattleGenerator
{
public:
	CustomBattleGenerator();
	~CustomBattleGenerator();

	CustomBattle* Generate(const KKSG::CustomBattleConfig& config, UINT64 roleid, bool issystem, UINT32 serverid);

};

#endif
