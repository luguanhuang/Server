using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/6/29 19:31:30


namespace XMainClient
{
    class Process_RpcC2G_FashionCompose
    {
        public static void OnReply(FashionComposeArg oArg, FashionComposeRes oRes)
        {
            if (oRes.result == ErrorCode.ERR_SUCCESS)
            {
            }
            else
            {
            }
        }

        public static void OnTimeout(FashionComposeArg oArg)
        {
        }
    }
}
