using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/4/14 13:48:01

namespace XMainClient
{
    class Process_RpcC2G_ChooseProfession
    {
        public static void OnReply(ChooseProfArg oArg, ChooseProfRes oRes)
        {
            if (oRes.errorcode == (int) KKSG.ErrorCode.ERR_SUCCESS)
            {
            }
        }

        public static void OnTimeout(ChooseProfArg oArg)
        {
        }
    }
}
