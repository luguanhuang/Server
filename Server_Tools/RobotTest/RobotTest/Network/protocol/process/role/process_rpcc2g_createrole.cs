using System;
using System.Collections.Generic;

using System.Text;
using System.IO;

using KKSG;

// generate by ProtoGen at date: 2014/10/13 11:56:44

namespace XMainClient
{
    class Process_RpcC2G_CreateRole
    {
        public static void OnReply(CreateRoleArg oArg, CreateRoleRes oRes)
        {
            /*if (oRes.result != ErrorCode.ERR_SUCCESS)
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

            oRes.roleData = new KKSG.RoleAllInfo();*/
        }

        public static void OnTimeout(CreateRoleArg oArg)
        {
        }
    }
}
