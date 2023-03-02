#ifndef __GSPCH_H__
#define __GSPCH_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#include <winsock.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "LogInit.h"
#include "timermgr.h"
#include "checkutil.h"

#include "pb/enum.pb.h"
#include "pb/project.pb.h"

#define ForeachCall(S, Fun, ...) do{ for(auto j = S.begin(); j != S.end(); ++j) {(*j)->Fun( __VA_ARGS__);}}while(0)
 
#define ArraySize(X) (sizeof(X)/sizeof(X[0]))


#endif