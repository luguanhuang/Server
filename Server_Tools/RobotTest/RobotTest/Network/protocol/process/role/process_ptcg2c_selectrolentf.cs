using System;
using System.Collections.Generic;
using System.Text;

// generate by ProtoGen at date: 2016/7/10 13:44:02

namespace XMainClient
{
	class Process_PtcG2C_SelectRoleNtf
	{
		public static void Process(PtcG2C_SelectRoleNtf roPtc)
		{
            LoginTask task = ConnectionMgr.CurrentConnection.GetTask() as LoginTask;
            if (task == null)
            {
                return;
            }

            task.OnSelectRoleReceiveData(roPtc.Data.roleData);
		}

	}
}