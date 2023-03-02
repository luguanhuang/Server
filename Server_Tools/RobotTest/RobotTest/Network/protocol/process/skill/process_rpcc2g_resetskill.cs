using System;
using System.Collections.Generic;
using KKSG;
// generate by ProtoGen at date: 2015/4/13 14:16:50


namespace XMainClient
{
    class Process_RpcC2G_ResetSkill
    {
        public static void OnReply(ResetSkillArg oArg, ResetSkillRes oRes)
        {
            if (oRes.errorcode == (int)KKSG.ErrorCode.ERR_SUCCESS)
            {
                
            }
        }

        public static void OnTimeout(ResetSkillArg oArg)
        {
        }
    }
}
