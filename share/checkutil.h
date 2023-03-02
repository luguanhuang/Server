#ifndef __H_Check_Util_H__
#define __H_Check_Util_H__


//
//DNA_LIVE_ENV:  线上环境中定义
//


#ifndef DNA_LIVE_ENV

#define CHECK_COND(cond) if(!(cond)) { LogError("Check cond: <"#cond"> failed"); assert(false); return; }
#define CHECK_COND_RETURN(cond, retVal) if(!(cond)) { LogError("Check cond: <"#cond"> failed"); assert(false); return retVal; }
#define CHECK_COND_WITH_LOG(cond, logerr) if(!(cond)) { logerr; assert(false); return; }
#define CHECK_COND_WITH_LOG_RETURN(cond, logerr, retVal) if(!(cond)) { logerr; assert(false); return retVal; }
#define CHECK_COND_NORETURN(cond) if(!(cond)) { LogError("Check cond: <"#cond"> failed"); assert(false); }

#else

#define CHECK_COND(cond) if(!(cond)) { LogWarn("Check cond: <"#cond"> failed"); return; }
#define CHECK_COND_RETURN(cond, retVal) if(!(cond)) { LogWarn("Check cond: <"#cond"> failed"); return retVal; }
#define CHECK_COND_WITH_LOG(cond, logerr) if(!(cond)) { logerr; return; }
#define CHECK_COND_WITH_LOG_RETURN(cond, logerr, retVal) if(!(cond)) { logerr; return retVal; }
#define CHECK_COND_NORETURN(cond) if(!(cond)) { LogError("Check cond: <"#cond"> failed"); }

#endif


#define GetVecElem(vec, index, defVal) ((index >= 0 && index < vec.size()) ? vec[index] : defVal)


#endif