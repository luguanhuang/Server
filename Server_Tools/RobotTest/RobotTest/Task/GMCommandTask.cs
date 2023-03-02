using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class GMCommandTask : Task
    {
        private string command;
        private bool isFinish;

        public GMCommandTask(string cmd)
        {
            command = cmd;
            isFinish = false;
        }

        public override void OnStart()
        {
            //Console.WriteLine("[" + state.Account + "] gm cmd: " + command);

            RpcC2G_GMCommand gmcmd = new RpcC2G_GMCommand();
            gmcmd.oArg.cmd = command;
            state.Send(gmcmd);
        }

        public override void OnUpdate()
        {
            
        }

        public void SetFinish()
        {
            isFinish = true;
        }

        public override bool IsFinish()
        {
            return isFinish;
        }

        public string GetCmd()
        {
            return command;
        }
        public override void ResetTask()
        {
            isFinish = false;
        }
    }
}
