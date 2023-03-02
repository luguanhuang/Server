using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 16:10:25

namespace XMainClient
{
	using ArgDataType = KKSG.BuyPartnerShopItemArg;
    using ResDataType = KKSG.BuyPartnerShopItemRes;

    class RpcC2M_BuyPartnerShopItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_BuyPartnerShopItem()
        {
        }

        public override uint GetRpcType()
        {
            return 14493;
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
            Process_RpcC2M_BuyPartnerShopItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_BuyPartnerShopItem.OnTimeout(oArg);
        }
    }
}
