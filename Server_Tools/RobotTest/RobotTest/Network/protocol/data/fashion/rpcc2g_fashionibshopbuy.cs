using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/9 11:53:39

namespace XMainClient
{
	using ArgDataType = KKSG.FashionIBShopBuyArg;
    using ResDataType = KKSG.FashionIBShopBuyRes;

    class RpcC2G_FashionIBShopBuy : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_FashionIBShopBuy()
        {
        }

        public override uint GetRpcType()
        {
            return 9349;
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
            Process_RpcC2G_FashionIBShopBuy.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FashionIBShopBuy.OnTimeout(oArg);
        }
    }
}
