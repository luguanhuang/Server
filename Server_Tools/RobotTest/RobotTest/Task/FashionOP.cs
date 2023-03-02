using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class FashionOP : Task
    {
        uint ItemID;
        uint op;

        public FashionOP(uint ItemID, uint op)
        {
            this.ItemID = ItemID;
            this.op = op;
        }

        public override void OnStart()
        {
            RpcC2G_UseItem rpc = new RpcC2G_UseItem();
            rpc.oArg.uid = this.ItemID;
            rpc.oArg.OpType = op;
            state.Send(rpc);
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
