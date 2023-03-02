#ifndef __LOGPCH_H__
#define __LOGPCH_H__

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
#include <Windows.h>
#include <winsock.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "LogInit.h"
#include "timermgr.h"


#endif