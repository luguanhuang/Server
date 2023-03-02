using System;
using System.Collections.Generic;
using System.Diagnostics;

using System.Text;
// generate by ProtoGen at date: 2014/10/16 11:10:54
using System.Xml.Schema;
using KKSG;

namespace XMainClient
{
	class Process_PtcG2C_EnterSceneNtf
	{
		public static void Process(PtcG2C_EnterSceneNtf roPtc)
		{
            Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] Enter Scene Notify, Scene: " + roPtc.Data.SceneID);

            ConnectionMgr.CurrentConnection.state.CurrentScene = roPtc.Data.SceneID;

            //PtcC2G_SceneLoadedNtf SceneLoadedPtc = new PtcC2G_SceneLoadedNtf();
            RpcC2G_DoEnterScene rpc = new RpcC2G_DoEnterScene();
            ConnectionMgr.CurrentConnection.Send(rpc);
            /*if (ConnectionMgr.CurrentConnection.state.GetCurrentTask().GetTaskType() == TaskType.TYPE_ENTERSCENE)
            {
                EnterSceneTask task = (EnterSceneTask)(ConnectionMgr.CurrentConnection.state.GetCurrentTask());
                task.Finish();
            }*/
		}
	}
}