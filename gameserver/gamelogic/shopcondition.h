#ifndef __SHOPCONDITION_H__
#define __SHOPCONDITION_H__

class Role;
struct ShopConditionParam;
struct ShopRoleParam;

class IShopCondition
{
public:
	virtual ~IShopCondition() {};
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param) = 0;
};

class LevelShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);

};

class PPTShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);

};

class DailyCountShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);

};

class WeekCountShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);

};

class CountShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);

};

class VIPShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);
	
};

class ArenaShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);
	
};

class PKShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);
	
};

class GuildShopCondition : public IShopCondition
{
	virtual UINT32 Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param);
	
};

#endif