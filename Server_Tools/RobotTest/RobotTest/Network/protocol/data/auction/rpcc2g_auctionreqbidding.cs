using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/11 17:04:39

namespace XMainClient
{
	using ArgDataType = KKSG.AuctBiddingData;
    using ResDataType = KKSG.AuctionReqResult;

    class RpcC2G_AuctionReqBidding : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AuctionReqBidding()
        {
        }

        public override uint GetRpcType()
        {
            return 17497;
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
            Process_RpcC2G_AuctionReqBidding.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AuctionReqBidding.OnTimeout(oArg);
        }
    }
}
