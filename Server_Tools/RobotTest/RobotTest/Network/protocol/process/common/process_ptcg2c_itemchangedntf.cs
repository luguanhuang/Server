using System;
using System.Collections.Generic;

using System.Text;

// generate by ProtoGen at date: 2015/1/23 11:46:55

namespace XMainClient
{
	class Process_PtcG2C_ItemChangedNtf
	{
		public static void Process(PtcG2C_ItemChangedNtf roPtc)
		{
            Bag bag = ConnectionMgr.CurrentConnection.state.GetBag();
            for (int i = 0; i < roPtc.Data.NewItems.Count; ++i)
            {
                bag.AddItem(roPtc.Data.NewItems[i]);
            }

            for (int i = 0; i < roPtc.Data.SwapItems.Count; ++i)
            {

            }

            for (int i = 0; i < roPtc.Data.RemoveItems.Count; ++i)
            {
                bag.RemoveItem(roPtc.Data.RemoveItems[i]);
            }
		}
	}
}