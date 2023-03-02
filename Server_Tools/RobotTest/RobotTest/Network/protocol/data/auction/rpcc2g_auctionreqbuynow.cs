using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/12 15:30:08

namespace XMainClient
{
	using ArgDataType = KKSG.auctReqBuyNowPara;
    using ResDataType = KKSG.AuctionReqResult;

    class RpcC2G_AuctionReqBuyNow : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AuctionReqBuyNow()
        {
        }

        public override uint GetRpcType()
        {
            return 15380;
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
            Process_RpcC2G_AuctionReqBuyNow.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AuctionReqBuyNow.OnTimeout(oArg);
        }
    }
}
