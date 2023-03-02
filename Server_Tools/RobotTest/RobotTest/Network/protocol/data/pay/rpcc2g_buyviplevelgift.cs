using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/22 22:36:00

namespace XMainClient
{
	using ArgDataType = KKSG.BuyVipLevelGiftArg;
    using ResDataType = KKSG.BuyVipLevelGiftRes;

    class RpcC2G_BuyVipLevelGift : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_BuyVipLevelGift()
        {
        }

        public override uint GetRpcType()
        {
            return 52536;
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
            Process_RpcC2G_BuyVipLevelGift.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyVipLevelGift.OnTimeout(oArg);
        }
    }
}
