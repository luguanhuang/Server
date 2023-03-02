#ifndef __SCENEKILL_H__
#define __SCENEKILL_H__

class Scene;
class Role;
class Enemy;
class SceneKill
{
public:
	SceneKill(Scene* scene);
	~SceneKill(){}

	void Init();

	void AddKillEnemyInfo(Enemy* enemy);

	int GetKillMonsterScore();

	void SetState(bool isEnd);

private:
	bool IsScoreEnemy(UINT32 enemyTemplateID);
	int CalcKillMonsterScore();


protected:
	Scene* m_pScene;
	std::map<UINT32, UINT32> m_killEnemyCount;
	std::map<UINT32, int> m_scoreEnemy;
	int m_killEnemyScore;
	bool m_isEnd;
};

#endif // 
