using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/6/15 16:58:48

namespace XMainClient
{
    class Process_RpcC2G_RefreshFashionChallenge
    {
        public static void OnReply(RefreshFashionArg oArg, RefreshFashionRes oRes)
        {
            if (oRes.result == ErrorCode.ERR_SUCCESS)
            {
            }
        }

        public static void OnTimeout(RefreshFashionArg oArg)
        {
        }
    }
}
