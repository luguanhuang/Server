#ifndef __RANDOMTASKCONFIG_H__
#define __RANDOMTASKCONFIG_H__

#include "table/RandomTaskTable.h"

class Role;
class Scene;
class IRandomTaskCondition;


class RandomTaskConfig
{
	RandomTaskConfig();
	~RandomTaskConfig();
	DECLARE_SINGLETON(RandomTaskConfig)

public:

	bool Init();
	void Uninit();

	//RandomTaskTable::RowData *GetConf(int taskID);
	//bool CheckTaskCondition(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
	//void GiveRandomTask(Role *pRole, Scene *pScene);

private:

	RandomTaskTable m_oTable;
	std::map<int, IRandomTaskCondition*> m_oTaskCondition;
	std::map<int, std::vector<int>> m_oTaskPool;
};

#endif // __RANDOMTASKCONFIG_H__