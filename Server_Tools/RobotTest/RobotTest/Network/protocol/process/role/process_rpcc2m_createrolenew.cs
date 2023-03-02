using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2016/7/9 19:22:28

namespace XMainClient
{
    class Process_RpcC2M_CreateRoleNew
    {
        public static void OnReply(CreateRoleNewArg oArg, CreateRoleNewRes oRes)
        {
            if (oRes.result != ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] Error: " + oRes.result.ToString());
                return;
            }

            LoginTask task = ConnectionMgr.CurrentConnection.GetTask() as LoginTask;
            if (task == null)
            {
                return;
            }

            task.OnCreateRole(oRes.result, oRes.roleData);

            oRes.roleData = new KKSG.RoleAllInfo();
        }

        public static void OnTimeout(CreateRoleNewArg oArg)
        {
        }
    }
}
