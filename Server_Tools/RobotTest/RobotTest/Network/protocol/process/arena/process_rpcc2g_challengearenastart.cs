using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/5/27 14:20:40

namespace XMainClient
{
    class Process_RpcC2G_ChallengeArenaStart
    {
        public static void OnReply(ChallengeArenaStartArg oArg, ChallengeArenaStartRes oRes)
        {
            if (oRes.ErrorCode != ErrorCode.ERR_SUCCESS)
            {
                if (oRes.ErrorCode == ErrorCode.ERR_SCENE_COOLDOWN)
                {
                }
                else
                {
                }
            }
        }

        public static void OnTimeout(ChallengeArenaStartArg oArg)
        {
        }
    }
}
