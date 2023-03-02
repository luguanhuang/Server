#include "pch.h"

/*
** Lua binding: tolua
** Generated automatically by tolua++-1.0.92 on 04/10/17 11:13:09.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_tolua_open (lua_State* tolua_S);

#include "tolua_global.h"
#include "unit/role.h"
#include "unit/unit.h"
#include "script/luauint64.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "define/tlogenum.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CUint64 (lua_State* tolua_S)
{
 CUint64* self = (CUint64*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_BagGiveItemTransition (lua_State* tolua_S)
{
 BagGiveItemTransition* self = (BagGiveItemTransition*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_BagTransition (lua_State* tolua_S)
{
 BagTransition* self = (BagTransition*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CUint64");
 tolua_usertype(tolua_S,"Role");
 tolua_usertype(tolua_S,"BagGiveItemTransition");
 tolua_usertype(tolua_S,"BagTransition");
 tolua_usertype(tolua_S,"Unit");
}

/* function: GetRoleByID */
#ifndef TOLUA_DISABLE_tolua_tolua_GetRoleByID00
static int tolua_tolua_GetRoleByID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CUint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUint64 id = *((CUint64*)  tolua_tousertype(tolua_S,1,0));
  {
   Role* tolua_ret = (Role*)  GetRoleByID(id);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Role");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoleByID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CUint64 */
#ifndef TOLUA_DISABLE_tolua_tolua_CUint64_new00
static int tolua_tolua_CUint64_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CUint64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CUint64* tolua_ret = (CUint64*)  Mtolua_new((CUint64)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CUint64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  CUint64 */
#ifndef TOLUA_DISABLE_tolua_tolua_CUint64_new00_local
static int tolua_tolua_CUint64_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CUint64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CUint64* tolua_ret = (CUint64*)  Mtolua_new((CUint64)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  CUint64 */
#ifndef TOLUA_DISABLE_tolua_tolua_CUint64_delete00
static int tolua_tolua_CUint64_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CUint64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUint64* self = (CUint64*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: tostr of class  CUint64 */
#ifndef TOLUA_DISABLE_tolua_tolua_CUint64_tostr00
static int tolua_tolua_CUint64_tostr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CUint64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUint64* self = (CUint64*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'tostr'", NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->tostr();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tostr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  CUint64 */
#ifndef TOLUA_DISABLE_tolua_tolua_CUint64_set00
static int tolua_tolua_CUint64_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CUint64",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CUint64* self = (CUint64*)  tolua_tousertype(tolua_S,1,0);
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'", NULL);
#endif
  {
   self->set(str);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLevel of class  Unit */
#ifndef TOLUA_DISABLE_tolua_tolua_Unit_GetLevel00
static int tolua_tolua_Unit_GetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Unit* self = (Unit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLevel'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetID of class  Unit */
#ifndef TOLUA_DISABLE_tolua_tolua_Unit_GetID00
static int tolua_tolua_Unit_GetID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Unit* self = (Unit*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetID'", NULL);
#endif
  {
   CUint64 tolua_ret = (CUint64)  self->GetID();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CUint64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CUint64));
     tolua_pushusertype(tolua_S,tolua_obj,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNickId of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_GetNickId00
static int tolua_tolua_Role_GetNickId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNickId'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->GetNickId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNickId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetSessionID of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_GetSessionID00
static int tolua_tolua_Role_GetSessionID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Role* self = (const Role*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetSessionID'", NULL);
#endif
  {
   CUint64 tolua_ret = (CUint64)  self->GetSessionID();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CUint64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CUint64));
     tolua_pushusertype(tolua_S,tolua_obj,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetSessionID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetRoleID of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_GetRoleID00
static int tolua_tolua_Role_GetRoleID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Role* self = (const Role*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetRoleID'", NULL);
#endif
  {
   CUint64 tolua_ret = (CUint64)  self->GetRoleID();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((CUint64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(CUint64));
     tolua_pushusertype(tolua_S,tolua_obj,"CUint64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetRoleID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_GetName00
static int tolua_tolua_Role_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Role* self = (const Role*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->GetName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAccount of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_GetAccount00
static int tolua_tolua_Role_GetAccount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAccount'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->GetAccount();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAccount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddDiamond of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_AddDiamond00
static int tolua_tolua_Role_AddDiamond00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
  int iCount = ((int)  tolua_tonumber(tolua_S,2,0));
  int reason = ((int)  tolua_tonumber(tolua_S,3,0));
  int subreason = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddDiamond'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->AddDiamond(iCount,reason,subreason);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddDiamond'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HintNotify of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_HintNotify00
static int tolua_tolua_Role_HintNotify00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
  unsigned int systemid = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  bool isremove = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HintNotify'", NULL);
#endif
  {
   self->HintNotify(systemid,isremove);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HintNotify'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsWatcher of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_IsWatcher00
static int tolua_tolua_Role_IsWatcher00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsWatcher'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsWatcher();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsWatcher'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddFriend of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_AddFriend00
static int tolua_tolua_Role_AddFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CUint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
  CUint64 friendid = *((CUint64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddFriend'", NULL);
#endif
  {
   self->AddFriend(friendid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DelFriend of class  Role */
#ifndef TOLUA_DISABLE_tolua_tolua_Role_DelFriend00
static int tolua_tolua_Role_DelFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Role",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CUint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* self = (Role*)  tolua_tousertype(tolua_S,1,0);
  CUint64 friendid = *((CUint64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DelFriend'", NULL);
#endif
  {
   self->DelFriend(friendid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DelFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  BagTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagTransition_new00
static int tolua_tolua_BagTransition_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BagTransition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* pRole = ((Role*)  tolua_tousertype(tolua_S,2,0));
  {
   BagTransition* tolua_ret = (BagTransition*)  Mtolua_new((BagTransition)(pRole));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BagTransition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  BagTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagTransition_new00_local
static int tolua_tolua_BagTransition_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BagTransition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* pRole = ((Role*)  tolua_tousertype(tolua_S,2,0));
  {
   BagTransition* tolua_ret = (BagTransition*)  Mtolua_new((BagTransition)(pRole));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BagTransition");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  BagTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagTransition_delete00
static int tolua_tolua_BagTransition_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BagTransition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BagTransition* self = (BagTransition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRecyle of class  BagTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagTransition_SetRecyle00
static int tolua_tolua_BagTransition_SetRecyle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BagTransition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BagTransition* self = (BagTransition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRecyle'", NULL);
#endif
  {
   self->SetRecyle();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRecyle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NotifyClient of class  BagTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagTransition_NotifyClient00
static int tolua_tolua_BagTransition_NotifyClient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BagTransition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BagTransition* self = (BagTransition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NotifyClient'", NULL);
#endif
  {
   self->NotifyClient();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NotifyClient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  BagGiveItemTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagGiveItemTransition_new00
static int tolua_tolua_BagGiveItemTransition_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BagGiveItemTransition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* pRole = ((Role*)  tolua_tousertype(tolua_S,2,0));
  {
   BagGiveItemTransition* tolua_ret = (BagGiveItemTransition*)  Mtolua_new((BagGiveItemTransition)(pRole));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BagGiveItemTransition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  BagGiveItemTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagGiveItemTransition_new00_local
static int tolua_tolua_BagGiveItemTransition_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BagGiveItemTransition",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Role",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Role* pRole = ((Role*)  tolua_tousertype(tolua_S,2,0));
  {
   BagGiveItemTransition* tolua_ret = (BagGiveItemTransition*)  Mtolua_new((BagGiveItemTransition)(pRole));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BagGiveItemTransition");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  BagGiveItemTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagGiveItemTransition_delete00
static int tolua_tolua_BagGiveItemTransition_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BagGiveItemTransition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BagGiveItemTransition* self = (BagGiveItemTransition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetReason of class  BagGiveItemTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagGiveItemTransition_SetReason00
static int tolua_tolua_BagGiveItemTransition_SetReason00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BagGiveItemTransition",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BagGiveItemTransition* self = (BagGiveItemTransition*)  tolua_tousertype(tolua_S,1,0);
  int nReason = ((int)  tolua_tonumber(tolua_S,2,0));
  int nSubReason = ((int)  tolua_tonumber(tolua_S,3,-1));
  bool bIsCheckZeroProfit = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetReason'", NULL);
#endif
  {
   self->SetReason(nReason,nSubReason,bIsCheckZeroProfit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetReason'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GiveItem of class  BagGiveItemTransition */
#ifndef TOLUA_DISABLE_tolua_tolua_BagGiveItemTransition_GiveItem00
static int tolua_tolua_BagGiveItemTransition_GiveItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BagGiveItemTransition",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BagGiveItemTransition* self = (BagGiveItemTransition*)  tolua_tousertype(tolua_S,1,0);
  int itemID = ((int)  tolua_tonumber(tolua_S,2,0));
  int itemCount = ((int)  tolua_tonumber(tolua_S,3,0));
  bool isbind = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GiveItem'", NULL);
#endif
  {
   self->GiveItem(itemID,itemCount,isbind);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GiveItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_tolua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"GetRoleByID",tolua_tolua_GetRoleByID00);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CUint64","CUint64","",tolua_collect_CUint64);
  #else
  tolua_cclass(tolua_S,"CUint64","CUint64","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CUint64");
   tolua_function(tolua_S,"new",tolua_tolua_CUint64_new00);
   tolua_function(tolua_S,"new_local",tolua_tolua_CUint64_new00_local);
   tolua_function(tolua_S,".call",tolua_tolua_CUint64_new00_local);
   tolua_function(tolua_S,"delete",tolua_tolua_CUint64_delete00);
   tolua_function(tolua_S,"tostr",tolua_tolua_CUint64_tostr00);
   tolua_function(tolua_S,"set",tolua_tolua_CUint64_set00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Unit","Unit","",NULL);
  tolua_beginmodule(tolua_S,"Unit");
   tolua_function(tolua_S,"GetLevel",tolua_tolua_Unit_GetLevel00);
   tolua_function(tolua_S,"GetID",tolua_tolua_Unit_GetID00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Role","Role","Unit",NULL);
  tolua_beginmodule(tolua_S,"Role");
   tolua_function(tolua_S,"GetNickId",tolua_tolua_Role_GetNickId00);
   tolua_function(tolua_S,"GetSessionID",tolua_tolua_Role_GetSessionID00);
   tolua_function(tolua_S,"GetRoleID",tolua_tolua_Role_GetRoleID00);
   tolua_function(tolua_S,"GetName",tolua_tolua_Role_GetName00);
   tolua_function(tolua_S,"GetAccount",tolua_tolua_Role_GetAccount00);
   tolua_function(tolua_S,"AddDiamond",tolua_tolua_Role_AddDiamond00);
   tolua_function(tolua_S,"HintNotify",tolua_tolua_Role_HintNotify00);
   tolua_function(tolua_S,"IsWatcher",tolua_tolua_Role_IsWatcher00);
   tolua_function(tolua_S,"AddFriend",tolua_tolua_Role_AddFriend00);
   tolua_function(tolua_S,"DelFriend",tolua_tolua_Role_DelFriend00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"BagTransition","BagTransition","",tolua_collect_BagTransition);
  #else
  tolua_cclass(tolua_S,"BagTransition","BagTransition","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"BagTransition");
   tolua_function(tolua_S,"new",tolua_tolua_BagTransition_new00);
   tolua_function(tolua_S,"new_local",tolua_tolua_BagTransition_new00_local);
   tolua_function(tolua_S,".call",tolua_tolua_BagTransition_new00_local);
   tolua_function(tolua_S,"delete",tolua_tolua_BagTransition_delete00);
   tolua_function(tolua_S,"SetRecyle",tolua_tolua_BagTransition_SetRecyle00);
   tolua_function(tolua_S,"NotifyClient",tolua_tolua_BagTransition_NotifyClient00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"BagGiveItemTransition","BagGiveItemTransition","BagTransition",tolua_collect_BagGiveItemTransition);
  #else
  tolua_cclass(tolua_S,"BagGiveItemTransition","BagGiveItemTransition","BagTransition",NULL);
  #endif
  tolua_beginmodule(tolua_S,"BagGiveItemTransition");
   tolua_function(tolua_S,"new",tolua_tolua_BagGiveItemTransition_new00);
   tolua_function(tolua_S,"new_local",tolua_tolua_BagGiveItemTransition_new00_local);
   tolua_function(tolua_S,".call",tolua_tolua_BagGiveItemTransition_new00_local);
   tolua_function(tolua_S,"delete",tolua_tolua_BagGiveItemTransition_delete00);
   tolua_function(tolua_S,"SetReason",tolua_tolua_BagGiveItemTransition_SetReason00);
   tolua_function(tolua_S,"GiveItem",tolua_tolua_BagGiveItemTransition_GiveItem00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"ItemFlow_Task",ItemFlow_Task);
  tolua_constant(tolua_S,"ItemFlow_Stage",ItemFlow_Stage);
  tolua_constant(tolua_S,"ItemFlow_Skill",ItemFlow_Skill);
  tolua_constant(tolua_S,"ItemFlow_Level",ItemFlow_Level);
  tolua_constant(tolua_S,"ItemFlow_Equip",ItemFlow_Equip);
  tolua_constant(tolua_S,"ItemFlow_Compose",ItemFlow_Compose);
  tolua_constant(tolua_S,"ItemFlow_Decompose",ItemFlow_Decompose);
  tolua_constant(tolua_S,"ItemFlow_Smelt",ItemFlow_Smelt);
  tolua_constant(tolua_S,"ItemFlow_Emblem",ItemFlow_Emblem);
  tolua_constant(tolua_S,"ItemFlow_Jade",ItemFlow_Jade);
  tolua_constant(tolua_S,"ItemFlow_Pay",ItemFlow_Pay);
  tolua_constant(tolua_S,"ItemFlow_Shop",ItemFlow_Shop);
  tolua_constant(tolua_S,"ItemFlow_Reward",ItemFlow_Reward);
  tolua_constant(tolua_S,"ItemFlow_Lottory",ItemFlow_Lottory);
  tolua_constant(tolua_S,"ItemFlow_Fatigue",ItemFlow_Fatigue);
  tolua_constant(tolua_S,"ItemFlow_Activity",ItemFlow_Activity);
  tolua_constant(tolua_S,"ItemFlow_Checkin",ItemFlow_Checkin);
  tolua_constant(tolua_S,"ItemFlow_Achievement",ItemFlow_Achievement);
  tolua_constant(tolua_S,"ItemFlow_Fashion",ItemFlow_Fashion);
  tolua_constant(tolua_S,"ItemFlow_Flower",ItemFlow_Flower);
  tolua_constant(tolua_S,"ItemFlow_TShow",ItemFlow_TShow);
  tolua_constant(tolua_S,"ItemFlow_Mail",ItemFlow_Mail);
  tolua_constant(tolua_S,"ItemFlow_FindBack",ItemFlow_FindBack);
  tolua_constant(tolua_S,"ItemFlow_Buy",ItemFlow_Buy);
  tolua_constant(tolua_S,"ItemFlow_GM",ItemFlow_GM);
  tolua_constant(tolua_S,"ItemFlow_Guild",ItemFlow_Guild);
  tolua_constant(tolua_S,"ItemFlow_GaoJiGu",ItemFlow_GaoJiGu);
  tolua_constant(tolua_S,"ItemFlow_Pet",ItemFlow_Pet);
  tolua_constant(tolua_S,"ItemFlow_Camp",ItemFlow_Camp);
  tolua_constant(tolua_S,"ItemFlow_Auction",ItemFlow_Auction);
  tolua_constant(tolua_S,"ItemFlow_AchieveV2",ItemFlow_AchieveV2);
  tolua_constant(tolua_S,"ItemFlow_Chat",ItemFlow_Chat);
  tolua_constant(tolua_S,"ItemFlow_IDIP",ItemFlow_IDIP);
  tolua_constant(tolua_S,"ItemFlow_QA",ItemFlow_QA);
  tolua_constant(tolua_S,"ItemFlow_DRAGON",ItemFlow_DRAGON);
  tolua_constant(tolua_S,"ItemFlow_Tower",ItemFlow_Tower);
  tolua_constant(tolua_S,"ItemFlow_Goddess",ItemFlow_Goddess);
  tolua_constant(tolua_S,"ItemFlow_Friend",ItemFlow_Friend);
  tolua_constant(tolua_S,"ItemFlow_Title",ItemFlow_Title);
  tolua_constant(tolua_S,"ItemFlow_FirstPass",ItemFlow_FirstPass);
  tolua_constant(tolua_S,"ItemFlow_ItemFindBack",ItemFlow_ItemFindBack);
  tolua_constant(tolua_S,"ItemFlow_BuyIBShop",ItemFlow_BuyIBShop);
  tolua_constant(tolua_S,"ItemFlow_Atlas",ItemFlow_Atlas);
  tolua_constant(tolua_S,"ItemFlow_Sprite",ItemFlow_Sprite);
  tolua_constant(tolua_S,"ItemFlow_SuperRisk",ItemFlow_SuperRisk);
  tolua_constant(tolua_S,"ItemFlow_LevelSeal",ItemFlow_LevelSeal);
  tolua_constant(tolua_S,"ItemFlow_UseItem",ItemFlow_UseItem);
  tolua_constant(tolua_S,"ItemFlow_OpenSystem",ItemFlow_OpenSystem);
  tolua_constant(tolua_S,"ItemFlow_SpActivity",ItemFlow_SpActivity);
  tolua_constant(tolua_S,"ItemFlow_Garden",ItemFlow_Garden);
  tolua_constant(tolua_S,"ItemFlow_GuildAuction",ItemFlow_GuildAuction);
  tolua_constant(tolua_S,"ItemFlow_GoldClick",ItemFlow_GoldClick);
  tolua_constant(tolua_S,"ItemFlow_MailBagFull",ItemFlow_MailBagFull);
  tolua_constant(tolua_S,"ItemFlow_TeamCost",ItemFlow_TeamCost);
  tolua_constant(tolua_S,"ItemFlow_GuildInherit",ItemFlow_GuildInherit);
  tolua_constant(tolua_S,"ItemFlow_Rename",ItemFlow_Rename);
  tolua_constant(tolua_S,"ItemFlow_Enchant",ItemFlow_Enchant);
  tolua_constant(tolua_S,"ItemFlow_Partner",ItemFlow_Partner);
  tolua_constant(tolua_S,"ItemFlow_PokerTournament",ItemFlow_PokerTournament);
  tolua_constant(tolua_S,"ItemFlow_GuildTerritory",ItemFlow_GuildTerritory);
  tolua_constant(tolua_S,"ItemFlow_HeroBattle",ItemFlow_HeroBattle);
  tolua_constant(tolua_S,"ItemFlow_Forge",ItemFlow_Forge);
  tolua_constant(tolua_S,"ItemFlow_ItemSell",ItemFlow_ItemSell);
  tolua_constant(tolua_S,"ItemFlow_ChangePro",ItemFlow_ChangePro);
  tolua_constant(tolua_S,"ItemFlow_LeagueBattle",ItemFlow_LeagueBattle);
  tolua_constant(tolua_S,"ItemFlow_Task_Finish",ItemFlow_Task_Finish);
  tolua_constant(tolua_S,"ItemFlow_Stage_Revive",ItemFlow_Stage_Revive);
  tolua_constant(tolua_S,"ItemFlow_Stage_Count",ItemFlow_Stage_Count);
  tolua_constant(tolua_S,"ItemFlow_Stage_Chest",ItemFlow_Stage_Chest);
  tolua_constant(tolua_S,"ItemFlow_Stage_Enter",ItemFlow_Stage_Enter);
  tolua_constant(tolua_S,"ItemFlow_Stage_Supplement",ItemFlow_Stage_Supplement);
  tolua_constant(tolua_S,"ItemFlow_Stage_TeamBox",ItemFlow_Stage_TeamBox);
  tolua_constant(tolua_S,"ItemFlow_Stage_Doodad",ItemFlow_Stage_Doodad);
  tolua_constant(tolua_S,"ItemFlow_Stage_Battle",ItemFlow_Stage_Battle);
  tolua_constant(tolua_S,"ItemFlow_Stage_Abysss",ItemFlow_Stage_Abysss);
  tolua_constant(tolua_S,"ItemFlow_Stage_Nest",ItemFlow_Stage_Nest);
  tolua_constant(tolua_S,"ItemFlow_Stage_WorldBoss",ItemFlow_Stage_WorldBoss);
  tolua_constant(tolua_S,"ItemFlow_Stage_GuildBoss",ItemFlow_Stage_GuildBoss);
  tolua_constant(tolua_S,"ItemFlow_Stage_Arena",ItemFlow_Stage_Arena);
  tolua_constant(tolua_S,"ItemFlow_Stage_Pk",ItemFlow_Stage_Pk);
  tolua_constant(tolua_S,"ItemFlow_Stage_BossRush",ItemFlow_Stage_BossRush);
  tolua_constant(tolua_S,"ItemFlow_Stage_ExpTeam",ItemFlow_Stage_ExpTeam);
  tolua_constant(tolua_S,"ItemFlow_Stage_Tower",ItemFlow_Stage_Tower);
  tolua_constant(tolua_S,"ItemFlow_Stage_Goblin",ItemFlow_Stage_Goblin);
  tolua_constant(tolua_S,"ItemFlow_Stage_Unknown",ItemFlow_Stage_Unknown);
  tolua_constant(tolua_S,"ItemFlow_Skill_Up",ItemFlow_Skill_Up);
  tolua_constant(tolua_S,"ItemFlow_Skill_Reset",ItemFlow_Skill_Reset);
  tolua_constant(tolua_S,"ItemFlow_Level_Give",ItemFlow_Level_Give);
  tolua_constant(tolua_S,"ItemFlow_Equip_Enhance",ItemFlow_Equip_Enhance);
  tolua_constant(tolua_S,"ItemFlow_Decompose_Item",ItemFlow_Decompose_Item);
  tolua_constant(tolua_S,"ItemFlow_Equip_Compose",ItemFlow_Equip_Compose);
  tolua_constant(tolua_S,"ItemFlow_Jade_Compose",ItemFlow_Jade_Compose);
  tolua_constant(tolua_S,"ItemFlow_Jade_OpenSlot",ItemFlow_Jade_OpenSlot);
  tolua_constant(tolua_S,"ItemFlow_Jade_BodyCompose",ItemFlow_Jade_BodyCompose);
  tolua_constant(tolua_S,"ItemFlow_Jade_Equip",ItemFlow_Jade_Equip);
  tolua_constant(tolua_S,"ItemFlow_Jade_Equip_Replace",ItemFlow_Jade_Equip_Replace);
  tolua_constant(tolua_S,"ItemFlow_Jade_TakeOff",ItemFlow_Jade_TakeOff);
  tolua_constant(tolua_S,"ItemFlow_Emblem_LevelUp",ItemFlow_Emblem_LevelUp);
  tolua_constant(tolua_S,"ItemFlow_Emblem_IdentifyCost",ItemFlow_Emblem_IdentifyCost);
  tolua_constant(tolua_S,"ItemFlow_Emblem_Compose",ItemFlow_Emblem_Compose);
  tolua_constant(tolua_S,"ItemFlow_Shop_Buy",ItemFlow_Shop_Buy);
  tolua_constant(tolua_S,"ItemFlow_Shop_Refresh",ItemFlow_Shop_Refresh);
  tolua_constant(tolua_S,"ItemFlow_Pay_Give",ItemFlow_Pay_Give);
  tolua_constant(tolua_S,"ItemFlow_Lottory_Draw",ItemFlow_Lottory_Draw);
  tolua_constant(tolua_S,"ItemFlow_Checkin_Day",ItemFlow_Checkin_Day);
  tolua_constant(tolua_S,"ItemFlow_Activity_Day",ItemFlow_Activity_Day);
  tolua_constant(tolua_S,"ItemFlow_Flower_Send",ItemFlow_Flower_Send);
  tolua_constant(tolua_S,"ItemFlow_Flower_Get",ItemFlow_Flower_Get);
  tolua_constant(tolua_S,"ItemFlow_Flower_GetRankReward",ItemFlow_Flower_GetRankReward);
  tolua_constant(tolua_S,"ItemFlow_Achievement_Give",ItemFlow_Achievement_Give);
  tolua_constant(tolua_S,"ItemFlow_Fashion_Compose",ItemFlow_Fashion_Compose);
  tolua_constant(tolua_S,"ItemFlow_Fashion_SkillPoint",ItemFlow_Fashion_SkillPoint);
  tolua_constant(tolua_S,"ItemFlow_Mail_Give",ItemFlow_Mail_Give);
  tolua_constant(tolua_S,"ItemFlow_FindBack_Exp",ItemFlow_FindBack_Exp);
  tolua_constant(tolua_S,"ItemFlow_TShow_Vote",ItemFlow_TShow_Vote);
  tolua_constant(tolua_S,"ItemFlow_Fatigue_Add",ItemFlow_Fatigue_Add);
  tolua_constant(tolua_S,"ItemFlow_Buy_Coin",ItemFlow_Buy_Coin);
  tolua_constant(tolua_S,"ItemFlow_Buy_Fatigue",ItemFlow_Buy_Fatigue);
  tolua_constant(tolua_S,"ItemFlow_Buy_DragonCoin",ItemFlow_Buy_DragonCoin);
  tolua_constant(tolua_S,"ItemFlow_GM_Give",ItemFlow_GM_Give);
  tolua_constant(tolua_S,"ItemFlow_GM_Clear",ItemFlow_GM_Clear);
  tolua_constant(tolua_S,"ItemFlow_Reward_System",ItemFlow_Reward_System);
  tolua_constant(tolua_S,"ItemFlow_Reward_Online",ItemFlow_Reward_Online);
  tolua_constant(tolua_S,"ItemFlow_Reward_NextDay",ItemFlow_Reward_NextDay);
  tolua_constant(tolua_S,"ItemFlow_Reward_Days",ItemFlow_Reward_Days);
  tolua_constant(tolua_S,"ItemFlow_Reward_Login",ItemFlow_Reward_Login);
  tolua_constant(tolua_S,"ItemFlow_Guild_Create",ItemFlow_Guild_Create);
  tolua_constant(tolua_S,"ItemFlow_Guild_Card",ItemFlow_Guild_Card);
  tolua_constant(tolua_S,"ItemFlow_Guild_Checkin",ItemFlow_Guild_Checkin);
  tolua_constant(tolua_S,"ItemFlow_Guild_Fatigue",ItemFlow_Guild_Fatigue);
  tolua_constant(tolua_S,"ItemFlow_Guild_Bonus",ItemFlow_Guild_Bonus);
  tolua_constant(tolua_S,"ItemFlow_GaoJiGu_Fish",ItemFlow_GaoJiGu_Fish);
  tolua_constant(tolua_S,"ItemFlow_Pet_Feed",ItemFlow_Pet_Feed);
  tolua_constant(tolua_S,"ItemFlow_Camp_Task",ItemFlow_Camp_Task);
  tolua_constant(tolua_S,"ItemFlow_Auction_Take",ItemFlow_Auction_Take);
  tolua_constant(tolua_S,"ItemFlow_Auction_Give",ItemFlow_Auction_Give);
  tolua_constant(tolua_S,"ItemFlow_AchieveV2_Achieve",ItemFlow_AchieveV2_Achieve);
  tolua_constant(tolua_S,"ItemFlow_AchieveV2_AchievePoint",ItemFlow_AchieveV2_AchievePoint);
  tolua_constant(tolua_S,"ItemFlow_Stage_Pvp",ItemFlow_Stage_Pvp);
  tolua_constant(tolua_S,"ItemFlow_Chat_World",ItemFlow_Chat_World);
  tolua_constant(tolua_S,"ItemFlow_IDIP_Oper",ItemFlow_IDIP_Oper);
  tolua_constant(tolua_S,"ItemFlow_QA_AnswerRight",ItemFlow_QA_AnswerRight);
  tolua_constant(tolua_S,"ItemFlow_DRAGON_Enter",ItemFlow_DRAGON_Enter);
  tolua_constant(tolua_S,"ItemFlow_Stage_Sweep",ItemFlow_Stage_Sweep);
  tolua_constant(tolua_S,"ItemFlow_Tower_Sweep",ItemFlow_Tower_Sweep);
  tolua_constant(tolua_S,"ItemFlow_Goddess_Reward",ItemFlow_Goddess_Reward);
  tolua_constant(tolua_S,"ItemFlow_Guild_Dare_Reward",ItemFlow_Guild_Dare_Reward);
  tolua_constant(tolua_S,"ItemFlow_NestBuy",ItemFlow_NestBuy);
  tolua_constant(tolua_S,"ItemFlow_AbysssBuy",ItemFlow_AbysssBuy);
  tolua_constant(tolua_S,"ItemFlow_FriendGift",ItemFlow_FriendGift);
  tolua_constant(tolua_S,"ItemFlow_EnhanceTrans",ItemFlow_EnhanceTrans);
  tolua_constant(tolua_S,"ItemFlow_Expand_Seat",ItemFlow_Expand_Seat);
  tolua_constant(tolua_S,"ItemFlow_BuyPay",ItemFlow_BuyPay);
  tolua_constant(tolua_S,"ItemFlow_BuyAileen",ItemFlow_BuyAileen);
  tolua_constant(tolua_S,"ItemFlow_BuyPayCard",ItemFlow_BuyPayCard);
  tolua_constant(tolua_S,"ItemFlow_BuyGrowthFund",ItemFlow_BuyGrowthFund);
  tolua_constant(tolua_S,"ItemFlow_PayCardAward",ItemFlow_PayCardAward);
  tolua_constant(tolua_S,"ItemFlow_TitleLevelUp",ItemFlow_TitleLevelUp);
  tolua_constant(tolua_S,"ItemFlow_PayFirstAward",ItemFlow_PayFirstAward);
  tolua_constant(tolua_S,"ItemFlow_FirstPassRank",ItemFlow_FirstPassRank);
  tolua_constant(tolua_S,"ItemFlow_FirstPassCommend",ItemFlow_FirstPassCommend);
  tolua_constant(tolua_S,"ItemFlow_VipLevelGift",ItemFlow_VipLevelGift);
  tolua_constant(tolua_S,"ItemFlow_GrowthFundAward",ItemFlow_GrowthFundAward);
  tolua_constant(tolua_S,"ItemFlow_ItemFindBackCost",ItemFlow_ItemFindBackCost);
  tolua_constant(tolua_S,"ItemFlow_ItemFindBackGet",ItemFlow_ItemFindBackGet);
  tolua_constant(tolua_S,"ItemFlow_BuyIBItemByDiamond",ItemFlow_BuyIBItemByDiamond);
  tolua_constant(tolua_S,"ItemFlow_BuyIBItemByDragonCoin",ItemFlow_BuyIBItemByDragonCoin);
  tolua_constant(tolua_S,"ItemFlow_CreateAtlasTeam",ItemFlow_CreateAtlasTeam);
  tolua_constant(tolua_S,"ItemFlow_SpriteLevelUp",ItemFlow_SpriteLevelUp);
  tolua_constant(tolua_S,"ItemFlow_SpriteEvolutionLevelUp",ItemFlow_SpriteEvolutionLevelUp);
  tolua_constant(tolua_S,"ItemFlow_SpriteAwake",ItemFlow_SpriteAwake);
  tolua_constant(tolua_S,"ItemFlow_SpriteDecompose",ItemFlow_SpriteDecompose);
  tolua_constant(tolua_S,"ItemFlow_SpriteLottery",ItemFlow_SpriteLottery);
  tolua_constant(tolua_S,"ItemFlow_GaoJiGu_UseFish",ItemFlow_GaoJiGu_UseFish);
  tolua_constant(tolua_S,"ItemFlow_SuperRiskDice",ItemFlow_SuperRiskDice);
  tolua_constant(tolua_S,"ItemFlow_LevelSeal_Exchange",ItemFlow_LevelSeal_Exchange);
  tolua_constant(tolua_S,"ItemFlow_SuperRiskOpenBox",ItemFlow_SuperRiskOpenBox);
  tolua_constant(tolua_S,"ItemFlow_SuperRiskBattle",ItemFlow_SuperRiskBattle);
  tolua_constant(tolua_S,"ItemFlow_SuperRiskAccelerate",ItemFlow_SuperRiskAccelerate);
  tolua_constant(tolua_S,"ItemFlow_SuperRiskBuy",ItemFlow_SuperRiskBuy);
  tolua_constant(tolua_S,"ItemFlow_SuperRiskRebattle",ItemFlow_SuperRiskRebattle);
  tolua_constant(tolua_S,"ItemFlow_StageDragonExp",ItemFlow_StageDragonExp);
  tolua_constant(tolua_S,"ItemFlow_SmeltEquip",ItemFlow_SmeltEquip);
  tolua_constant(tolua_S,"ItemFlow_SmeltEmblem",ItemFlow_SmeltEmblem);
  tolua_constant(tolua_S,"ItemFlow_Tower_RefreshSweep",ItemFlow_Tower_RefreshSweep);
  tolua_constant(tolua_S,"ItemFlow_Tower_FirstPass",ItemFlow_Tower_FirstPass);
  tolua_constant(tolua_S,"ItemFlow_BuyArenaCost",ItemFlow_BuyArenaCost);
  tolua_constant(tolua_S,"ItemFlow_ClearArenaCD",ItemFlow_ClearArenaCD);
  tolua_constant(tolua_S,"ItemFlow_ResetProfession",ItemFlow_ResetProfession);
  tolua_constant(tolua_S,"ItemFlow_Buy_BlueBird",ItemFlow_Buy_BlueBird);
  tolua_constant(tolua_S,"ItemFlow_UsePeck",ItemFlow_UsePeck);
  tolua_constant(tolua_S,"ItemFlow_CircleDraw",ItemFlow_CircleDraw);
  tolua_constant(tolua_S,"ItemFlow_UsePetBorn",ItemFlow_UsePetBorn);
  tolua_constant(tolua_S,"ItemFlow_UseSceneCountTicket",ItemFlow_UseSceneCountTicket);
  tolua_constant(tolua_S,"ItemFlow_OpenSystemGive",ItemFlow_OpenSystemGive);
  tolua_constant(tolua_S,"ItemFlow_SpActivity_GetReward",ItemFlow_SpActivity_GetReward);
  tolua_constant(tolua_S,"ItemFlow_SpActivity_BigPrize",ItemFlow_SpActivity_BigPrize);
  tolua_constant(tolua_S,"ItemFlow_GuildCardMatch",ItemFlow_GuildCardMatch);
  tolua_constant(tolua_S,"ItemFlow_Auction_GetBack",ItemFlow_Auction_GetBack);
  tolua_constant(tolua_S,"ItemFlow_LevelSealSelfGit",ItemFlow_LevelSealSelfGit);
  tolua_constant(tolua_S,"ItemFlow_Garden_PlantSeed",ItemFlow_Garden_PlantSeed);
  tolua_constant(tolua_S,"ItemFlow_Garden_Fish",ItemFlow_Garden_Fish);
  tolua_constant(tolua_S,"ItemFlow_Garden_Cooking",ItemFlow_Garden_Cooking);
  tolua_constant(tolua_S,"ItemFlow_Garden_Banquet",ItemFlow_Garden_Banquet);
  tolua_constant(tolua_S,"ItemFlow_Guild_Wage",ItemFlow_Guild_Wage);
  tolua_constant(tolua_S,"ItemFlow_GuildAuct_Take",ItemFlow_GuildAuct_Take);
  tolua_constant(tolua_S,"ItemFlow_GoldClickGive",ItemFlow_GoldClickGive);
  tolua_constant(tolua_S,"ItemFlow_GoldClickTake",ItemFlow_GoldClickTake);
  tolua_constant(tolua_S,"ItemFlow_UsePandora",ItemFlow_UsePandora);
  tolua_constant(tolua_S,"ItemFlow_GuildAuct_Give",ItemFlow_GuildAuct_Give);
  tolua_constant(tolua_S,"ItemFlow_Donate",ItemFlow_Donate);
  tolua_constant(tolua_S,"ItemFlow_TeamCostTake",ItemFlow_TeamCostTake);
  tolua_constant(tolua_S,"ItemFlow_TeamCostGive",ItemFlow_TeamCostGive);
  tolua_constant(tolua_S,"ItemFlow_GuildInheritGive",ItemFlow_GuildInheritGive);
  tolua_constant(tolua_S,"ItemFlow_RenameCost",ItemFlow_RenameCost);
  tolua_constant(tolua_S,"ItemFlow_GuildBossAward",ItemFlow_GuildBossAward);
  tolua_constant(tolua_S,"ItemFlow_GuildArenaAward",ItemFlow_GuildArenaAward);
  tolua_constant(tolua_S,"ItemFlow_GuildRankdAward",ItemFlow_GuildRankdAward);
  tolua_constant(tolua_S,"ItemFlow_GuildResWarAward",ItemFlow_GuildResWarAward);
  tolua_constant(tolua_S,"ItemFlow_GuildCampRankAward",ItemFlow_GuildCampRankAward);
  tolua_constant(tolua_S,"ItemFlow_SkyCityAward",ItemFlow_SkyCityAward);
  tolua_constant(tolua_S,"ItemFlow_GuildCardAward",ItemFlow_GuildCardAward);
  tolua_constant(tolua_S,"ItemFlow_FlowerWeekRankAward",ItemFlow_FlowerWeekRankAward);
  tolua_constant(tolua_S,"ItemFlow_GuildLadderAward",ItemFlow_GuildLadderAward);
  tolua_constant(tolua_S,"ItemFlow_QAAward",ItemFlow_QAAward);
  tolua_constant(tolua_S,"ItemFlow_WorldBossAward",ItemFlow_WorldBossAward);
  tolua_constant(tolua_S,"ItemFlow_PkWeekRankAward",ItemFlow_PkWeekRankAward);
  tolua_constant(tolua_S,"ItemFlow_PlatFriendGift",ItemFlow_PlatFriendGift);
  tolua_constant(tolua_S,"ItemFlow_DragonExpRankAward",ItemFlow_DragonExpRankAward);
  tolua_constant(tolua_S,"ItemFlow_DonateMemberItem",ItemFlow_DonateMemberItem);
  tolua_constant(tolua_S,"ItemFlow_GuildAuctProfit",ItemFlow_GuildAuctProfit);
  tolua_constant(tolua_S,"ItemFlow_GuildAuctFailBack",ItemFlow_GuildAuctFailBack);
  tolua_constant(tolua_S,"ItemFlow_TeamCostAward",ItemFlow_TeamCostAward);
  tolua_constant(tolua_S,"ItemFlow_QQVip",ItemFlow_QQVip);
  tolua_constant(tolua_S,"ItemFlow_PayMember",ItemFlow_PayMember);
  tolua_constant(tolua_S,"ItemFlow_TeamCostBack",ItemFlow_TeamCostBack);
  tolua_constant(tolua_S,"ItemFlow_PetSkillBook",ItemFlow_PetSkillBook);
  tolua_constant(tolua_S,"ItemFlow_Enchant_Take",ItemFlow_Enchant_Take);
  tolua_constant(tolua_S,"ItemFlow_MakePartner",ItemFlow_MakePartner);
  tolua_constant(tolua_S,"ItemFlow_WeekNest",ItemFlow_WeekNest);
  tolua_constant(tolua_S,"ItemFlow_PartnerChest",ItemFlow_PartnerChest);
  tolua_constant(tolua_S,"ItemFlow_PokerTournamentAward",ItemFlow_PokerTournamentAward);
  tolua_constant(tolua_S,"ItemFlow_PartnerShop",ItemFlow_PartnerShop);
  tolua_constant(tolua_S,"ItemFlow_PokerTournamentCost",ItemFlow_PokerTournamentCost);
  tolua_constant(tolua_S,"ItemFlow_MentorComplete",ItemFlow_MentorComplete);
  tolua_constant(tolua_S,"ItemFlow_GuildTerrChall",ItemFlow_GuildTerrChall);
  tolua_constant(tolua_S,"ItemFlow_GuildTerrJoin",ItemFlow_GuildTerrJoin);
  tolua_constant(tolua_S,"ItemFlow_HeroBattle_Reward",ItemFlow_HeroBattle_Reward);
  tolua_constant(tolua_S,"ItemFlow_HeroBattle_WeekReward",ItemFlow_HeroBattle_WeekReward);
  tolua_constant(tolua_S,"ItemFlow_HeroBattle_BuyHero",ItemFlow_HeroBattle_BuyHero);
  tolua_constant(tolua_S,"ItemFlow_SpActivity_Collect",ItemFlow_SpActivity_Collect);
  tolua_constant(tolua_S,"ItemFlow_Desig",ItemFlow_Desig);
  tolua_constant(tolua_S,"ItemFlow_Forge_Take",ItemFlow_Forge_Take);
  tolua_constant(tolua_S,"ItemFlow_Exchange_Dragoncoin",ItemFlow_Exchange_Dragoncoin);
  tolua_constant(tolua_S,"ItemFlow_ItemSell_Out",ItemFlow_ItemSell_Out);
  tolua_constant(tolua_S,"ItemFlow_ItemSell_In",ItemFlow_ItemSell_In);
  tolua_constant(tolua_S,"ItemFlow_Fashion_TimeOut",ItemFlow_Fashion_TimeOut);
  tolua_constant(tolua_S,"ItemFlow_ChangePro_Cost",ItemFlow_ChangePro_Cost);
  tolua_constant(tolua_S,"ItemFlow_ChangePro_ItemTake",ItemFlow_ChangePro_ItemTake);
  tolua_constant(tolua_S,"ItemFlow_ChangePro_ItemGive",ItemFlow_ChangePro_ItemGive);
  tolua_constant(tolua_S,"ItemFlow_SpriteTrain",ItemFlow_SpriteTrain);
  tolua_constant(tolua_S,"ItemFlow_SpriteResetTrain",ItemFlow_SpriteResetTrain);
  tolua_constant(tolua_S,"ItemFlow_LeagueBattleRace",ItemFlow_LeagueBattleRace);
  tolua_constant(tolua_S,"ItemFlow_SkillTwo",ItemFlow_SkillTwo);
  tolua_constant(tolua_S,"ItemFlow_PayReward",ItemFlow_PayReward);
  tolua_constant(tolua_S,"ItemFlow_MentorTask",ItemFlow_MentorTask);
  tolua_constant(tolua_S,"ItemFlow_FlowerValentinesDayRankAward",ItemFlow_FlowerValentinesDayRankAward);
  tolua_constant(tolua_S,"ItemFlow_Compose_Common",ItemFlow_Compose_Common);
  tolua_constant(tolua_S,"ItemFlow_Team_UseTicket",ItemFlow_Team_UseTicket);
  tolua_constant(tolua_S,"ItemFlow_Stage_ActivityOne",ItemFlow_Stage_ActivityOne);
  tolua_constant(tolua_S,"ItemFlow_Stage_ActivityTwo",ItemFlow_Stage_ActivityTwo);
  tolua_constant(tolua_S,"ItemFlow_Stage_ActivityThree",ItemFlow_Stage_ActivityThree);
  tolua_constant(tolua_S,"ItemFlow_SpActivityPartner_GetReward",ItemFlow_SpActivityPartner_GetReward);
  tolua_constant(tolua_S,"ItemFlow_SpActivityBackFlow_GetReward",ItemFlow_SpActivityBackFlow_GetReward);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_tolua (lua_State* tolua_S) {
 return tolua_tolua_open(tolua_S);
};
#endif

