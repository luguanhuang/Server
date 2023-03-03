/* This file is generated by tdr. */
/* No manual modification is permitted. */

/* tdr version: 2.7.12 */

#ifndef TSF4G_TDR_TDR_SERVICE_H_
#define TSF4G_TDR_TDR_SERVICE_H_

#include "TdrObject.h"
#include <string>

namespace tsf4g_tdr
{

// base class for all Closures
class ClosureBase
{
    public:
        virtual ~ClosureBase() {}
        virtual bool IsSelfDelete() const = 0;
};

// primary template
template <
        typename R,
        typename Arg1 = void,
        typename Arg2 = void,
        typename Arg3 = void,
        typename Arg4 = void,
        typename Arg5 = void,
        typename Arg6 = void,
        typename Arg7 = void,
        typename Arg8 = void,
        typename Arg9 = void,
        typename Arg10 = void,
        typename Arg11 = void,
        typename Arg12 = void,
        typename Arg13 = void,
        typename Arg14 = void,
        typename Arg15 = void,
        typename Arg16 = void,
        typename Arg17 = void,
        typename Arg18 = void,
        typename Arg19 = void,
        typename Arg20 = void
>
class Closure : public ClosureBase
{
    public:
        virtual R Run(
                    Arg1 arg1,
                    Arg2 arg2,
                    Arg3 arg3,
                    Arg4 arg4,
                    Arg5 arg5,
                    Arg6 arg6,
                    Arg7 arg7,
                    Arg8 arg8,
                    Arg9 arg9,
                    Arg10 arg10,
                    Arg11 arg11,
                    Arg12 arg12,
                    Arg13 arg13,
                    Arg14 arg14,
                    Arg15 arg15,
                    Arg16 arg16,
                    Arg17 arg17,
                    Arg18 arg18,
                    Arg19 arg19,
                    Arg20 arg20
                        ) = 0;
};


// specified for 0 argument
template <typename R>
class Closure<R> : public ClosureBase
{
    public:
        virtual R Run() = 0;
};

// specified for 1 argument
template <typename R, typename Arg1>
class Closure<R, Arg1> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1) = 0;
};

// specified for 2 arguments
template <typename R, typename Arg1, typename Arg2>
class Closure<R, Arg1, Arg2> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2) = 0;
};

// specified for 3 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3>
class Closure<R, Arg1, Arg2, Arg3> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3) = 0;
};

// specified for 4 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class Closure<R, Arg1, Arg2, Arg3, Arg4> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) = 0;
};

// specified for 5 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) = 0;
};

// specified for 6 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) = 0;
};

// specified for 7 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) = 0;
};

// specified for 8 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) = 0;
};

// specified for 9 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) = 0;
};

// specified for 10 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) = 0;
};

// specified for 11 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11) = 0;
};

// specified for 12 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12) = 0;
};

// specified for 13 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13) = 0;
};

// specified for 14 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13, Arg14 arg14) = 0;
};

// specified for 15 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13, Arg14 arg14, Arg15 arg15) = 0;
};

// specified for 16 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13, Arg14 arg14, Arg15 arg15, Arg16 arg16) = 0;
};

// specified for 17 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13, Arg14 arg14, Arg15 arg15, Arg16 arg16, Arg17 arg17) = 0;
};

// specified for 18 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13, Arg14 arg14, Arg15 arg15, Arg16 arg16, Arg17 arg17, Arg18 arg18) = 0;
};

// specified for 19 arguments
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
class Closure<R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19> : public ClosureBase
{
    public:
        virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11, Arg12 arg12, Arg13 arg13, Arg14 arg14, Arg15 arg15, Arg16 arg16, Arg17 arg17, Arg18 arg18, Arg19 arg19) = 0;
};

typedef Closure<void> IClosure;

/* RpcClient interface class */
class IRpcClient {
    public:
        IRpcClient(){}
        virtual ~IRpcClient(){}

    public:
        virtual int CallMethod(const char* serviceName,
                            const char* methodName,
                            const ITdrObject* request,
                            ITdrObject* response,
                            IClosure* done) = 0;
};

/* Service Stub base class */
class ServiceStub {
    public:
        ServiceStub(const char* svcName, IRpcClient* rpcClient) :
            serviceName(svcName), rpcClient(rpcClient) {}
        virtual ~ServiceStub() {}

    public:
        inline std::string service_name() const {
            return serviceName;
        }

        inline IRpcClient* rpc_client() const {
            return rpcClient;
        }

    private:
        std::string serviceName;
        IRpcClient* rpcClient;
};

/* Service Servant base class */
class ServiceServant {
    public:
        ServiceServant(const char* svcName) :
            serviceName(svcName) {}
        virtual ~ServiceServant() {}

    public:
        virtual int CallMethod(const char* methodName,
                            const ITdrObject* request,
                            ITdrObject* response,
                            IClosure* done) {return -1;}
        virtual ITdrObject* NewRequestObject(const char* methodName) {return NULL;}
        virtual ITdrObject* NewResponseObject(const char* methodName) {return NULL;}

    public:
        inline std::string service_name() const {
            return serviceName;
        }

    private:
        std::string serviceName;
};

inline int GetIdxByMethodName(const char* methodName,
                            const char* sortedMethodName[],
                            size_t count)
{
    int pos = -1;
    int begin = 0;
    int end = count - 1;
    while (begin <= end)
    {
        size_t mid = begin + (end - begin) / 2;
        if (strcmp(methodName, sortedMethodName[mid]) > 0)
        {
            begin = mid + 1;
        }
        else if (strcmp(methodName, sortedMethodName[mid]) < 0)
        {
            end = mid - 1;
        }
        else
        {
            pos = mid;
            break;
        }
    }

    return pos;
}

}
#endif
