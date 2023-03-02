#ifndef __SUPERRISK_H__
#define __SUPERRISK_H__

namespace KKSG
{
	class PlayDiceOverRes;
	class RoleRiskInfo;
}

/*enum RiskFlow
{
	RiskFlow_Null,
	RiskFlow_PlayDice,
	RiskFlow_PlayOver,
	RiskFlow_InBattle,
	RiskFlow_BattleOver,
	RiskFlow_CanBuy,
	RiskFlow_BuyOver,
};*/

class Role;
class Scene;
class SuperRisk
{
public:
	SuperRisk(Role* role);
	~SuperRisk();
	
	bool PlayDice(int mapID, int randValue, int& finalValue);

	bool DiceOver(KKSG::PlayDiceOverRes& roRes, bool forceOver=false);

	void OnNormalDiceOver();

	void ClearDiceInfo();

	bool CheckTriggerBuy(int gridtype);

	UINT32 RiskBuy();

	//Ã°ÏÕ¹Ø¿¨
	void GiveReward(std::map<int, int>& reward);

	void OnLeave(Scene* scene);

	bool CheckReBattle();

	UINT32 ReEnterBattle();

	void SetInDice(bool dice){inDice = dice;}

	bool DebugSetDiceNum(int num);

	void Load(const KKSG::RoleRiskInfo* info);
	void Save(KKSG::RoleRiskInfo* info);

	void Change();

private:
	Role* m_role;

	bool inDice;
	bool receiveOver;

	int getvalue;

	int debugDiceNum;

	//save
	int mapID;
	int gridType;
	int sceneID;
	bool canBuy;
};

#endif