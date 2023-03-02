using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2014/10/13 17:27:25

namespace XMainClient
{
    class Process_RpcC2G_ClientSelectRole
    {
        public static void OnReply(SelectRoleArg oArg, SelectRoleRes oRes)
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

            task.OnSelectRole(oRes.result, oRes.roleData);

            oRes.roleData = new KKSG.RoleAllInfo();*/
        }

        public static void OnTimeout(SelectRoleArg oArg)
        {
        }
    }
}
