using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class LogoutTask : Task
    {
        public override void OnStart()
        {
            Console.WriteLine("[" + state.Account + "] Logout");
            state.Close();
        }

        public override void OnUpdate()
        {
        }

        public override bool IsFinish()
        {
            return true;
        }
        public override void ResetTask()
        {
        }
    }
}
