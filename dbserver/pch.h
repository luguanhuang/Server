#ifndef __DBPCH_H__
#define __DBPCH_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
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
#include "xdelegate.hpp"

#include "pb/enum.pb.h"
#include "pb/project.pb.h"

using namespace std::placeholders;

//#define SOLODB

#endif
