using System;
using System.Collections.Generic;

using System.Text;

// generate by ProtoGen at date: 2014/11/11 12:04:14

using UnityEngine;
using XUtliPoolLib;
namespace XMainClient
{
	class Process_PtcG2C_MoveNotify
	{
		public static void Process(PtcG2C_MoveNotify roPtc)
		{
            Connection conn = ConnectionMgr.CurrentConnection;
            if (conn.state.GetCurrentTask().GetTaskType() == TaskType.TYPE_TEAMTASK)
            {
                TeamBattleTask task = ((TeamBattleTask)conn.state.GetCurrentTask());

                if (task.otherMemberID != roPtc.Data.RoleID)
                {
                    //Console.WriteLine(conn.state.Account + " team battle failed, rev id:" + roPtc.Data.RoleID + " member id: " + task.otherMemberID);
                    return;
                }
                PtcC2G_MoveOperationReq movePtc = new PtcC2G_MoveOperationReq();
                KKSG.MoveInfo info = roPtc.Data;
                Vector3 des = new Vector3(info.DesX, info.DesY, info.DesZ);
                Vector3 src = new Vector3(info.PosX, info.PosY, info.PosZ);
                Vector3 dir = des - src;


                //conn.state.MoveTo(info.DesX, info.DesZ);
                movePtc.Data.Direction = (int)(XCommon.singleton.AngleToFloat(dir) * 10);
                movePtc.Data.Velocity = (int)(4 * 10);
                movePtc.Data.PosX = (int)(src.x * 100);
                movePtc.Data.PosY = (int)(src.y * 100);
                movePtc.Data.PosZ = (int)(src.z * 100);
                movePtc.Data.DesX = (int)(des.x * 100);
                movePtc.Data.DesY = (int)(des.y * 100);
                movePtc.Data.DesZ = (int)(des.z * 100);
                movePtc.Data.Inertia = false;

                conn.state.Send(movePtc);
                //Console.WriteLine(conn.state.Account + " team battle success, rev id:" + roPtc.Data.RoleID + " member id: " + task.otherMemberID);
                //Console.WriteLine(conn.state.Account + " team battle success expID: " + task.expID + " state: "+ task.GetState() + " move to :"+des.x + " " + des.y + " " + des.z);
            }
            

		}
	}
}