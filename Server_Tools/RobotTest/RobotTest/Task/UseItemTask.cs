using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class UseItemTask : Task
    {
        uint ItemID;

        public UseItemTask(uint ItemID)
        {
            this.ItemID = ItemID;
        }

        private void UseItem(XItem item)
        {
            if (item != null)
            {
                Console.WriteLine("[" + state.Account + "] use Item: {0}-{1:X}", item.itemID, item.uid);

                RpcC2G_UseItem rpc = new RpcC2G_UseItem();
                rpc.oArg.uid = item.uid;
                rpc.oArg.OpType = 0;
                state.Send(rpc);
            }
        }

        public override void OnStart()
        {
            if (ItemID == 0)
            {
                IEnumerator<XItem> iter = state.GetBag().GetBagItemEnum();
                while (iter.MoveNext())
                {
                    UseItem(iter.Current);
                }
            }
            else
            {
                XItem item = state.GetBag().FindItem(ItemID);
                UseItem(item);
            }
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
