using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using KKSG;

namespace XMainClient
{
    class XItem
    {
        public ulong uid;
        public uint itemID;
    }

    class Bag
    {
        ClientState state;

        XItem[] EquipBag;
        XItem[] Fashion;
        XItem[] Emblem;
        List<XItem> NormalBag;

        public Bag(ClientState state_)
        {
            state = state_;
            EquipBag = new XItem[(int)EquipPosition.EquipCount];
            Fashion = new XItem[(int)FashionPosition.FashionCount];
            Emblem = new XItem[(int)EmblemPosition.EmblemCount];
            NormalBag = new List<XItem>();
        }

        public XItem FindItem(uint ItemID)
        {
            foreach (XItem i in NormalBag)
            {
                if (i.itemID == ItemID)
                {
                    return i;
                }
            }

            return null;
        }

        public IEnumerator<XItem> GetBagItemEnum()
        {
            return NormalBag.GetEnumerator();
        }

        public XItem MakeItem(Item item)
        {
            XItem xitem = new XItem();
            xitem.uid = item.uid;
            xitem.itemID = item.ItemID;
            return xitem;
        }

        public void AddItem(Item item)
        {
            Console.WriteLine("[" + state.Account + "] add item: " + item.ItemID);
            NormalBag.Add(MakeItem(item));
        }

        public void RemoveItem(ulong uid)
        {
            for (int i = 0; i < NormalBag.Count; ++i)
            {
                if (NormalBag[i].uid == uid)
                {
                    NormalBag.RemoveAt(i);
                    return;
                }
            }
        }

        public void InitItems(KKSG.BagContent bag)
        {
            for (int i = 0; i < bag.Equips.Count; ++i)
            {
                if (bag.Equips[i].uid != 0)
                {
                    EquipBag[i] = MakeItem(bag.Equips[i]);
                }
            }

            for (int i = 0; i < bag.Fashions.Count; ++i)
            {
                if (bag.Fashions[i].uid != 0)
                {
                    Fashion[i] = MakeItem(bag.Fashions[i]);
                }
            }

            for (int i = 0; i < bag.Emblems.Count; ++i)
            {
                if (bag.Emblems[i].uid != 0)
                {
                    Emblem[i] = MakeItem(bag.Emblems[i]);
                }
            }

            for (int i = 0; i < bag.Items.Count; ++i)
            {
                NormalBag.Add(MakeItem(bag.Items[i]));
            }
        }
    }
}
