#include "pch.h"

/*
** Lua binding: tolua
** Generated automatically by tolua++-1.0.92 on 04/10/17 11:37:52.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_tolua_open (lua_State* tolua_S);

#include "tolua_global.h"
#include "role/role.h"
#include "script/luauint64.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CUint64 (lua_State* tolua_S)
{
 CUint64* self = (CUint64*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CUint64");
 tolua_usertype(tolua_S,"CRole");
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
   CRole* tolua_ret = (CRole*)  GetRoleByID(id);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CRole");
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

/* method: GetID of class  CRole */
#ifndef TOLUA_DISABLE_tolua_tolua_CRole_GetID00
static int tolua_tolua_CRole_GetID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CRole* self = (const CRole*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetSessionID of class  CRole */
#ifndef TOLUA_DISABLE_tolua_tolua_CRole_GetSessionID00
static int tolua_tolua_CRole_GetSessionID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CRole* self = (const CRole*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetName of class  CRole */
#ifndef TOLUA_DISABLE_tolua_tolua_CRole_GetName00
static int tolua_tolua_CRole_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const CRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const CRole* self = (const CRole*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetAccount of class  CRole */
#ifndef TOLUA_DISABLE_tolua_tolua_CRole_GetAccount00
static int tolua_tolua_CRole_GetAccount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CRole",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CRole* self = (CRole*)  tolua_tousertype(tolua_S,1,0);
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
  tolua_cclass(tolua_S,"CRole","CRole","",NULL);
  tolua_beginmodule(tolua_S,"CRole");
   tolua_function(tolua_S,"GetID",tolua_tolua_CRole_GetID00);
   tolua_function(tolua_S,"GetSessionID",tolua_tolua_CRole_GetSessionID00);
   tolua_function(tolua_S,"GetName",tolua_tolua_CRole_GetName00);
   tolua_function(tolua_S,"GetAccount",tolua_tolua_CRole_GetAccount00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_tolua (lua_State* tolua_S) {
 return tolua_tolua_open(tolua_S);
};
#endif

