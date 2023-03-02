using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/13 17:39:40

namespace XMainClient
{
	using ArgDataType = KKSG.BuyShopItemArg;
    using ResDataType = KKSG.BuyShopItemRes;

    class RpcC2G_BuyShopItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_BuyShopItem()
        {
        }

        public override uint GetRpcType()
        {
            return 33881;
        }

        public override void Serialize(MemoryStream stream)
        {
            Serializer.Serialize(stream, oArg);
        }

        public override void DeSerialize(MemoryStream stream)
        {
            oRes = Serializer.Deserialize<ResDataType>(stream);
        }

        public override void Process()
        {
            base.Process();
            Process_RpcC2G_BuyShopItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyShopItem.OnTimeout(oArg);
        }
    }
}
