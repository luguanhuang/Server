using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/11 16:39:44

namespace XMainClient
{
	using ArgDataType = KKSG.AuctionRMBPara;
    using ResDataType = KKSG.AuctMyBiddingRet;

    class RpcC2G_AuctionReqMyBidding : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AuctionReqMyBidding()
        {
        }

        public override uint GetRpcType()
        {
            return 18355;
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
            Process_RpcC2G_AuctionReqMyBidding.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AuctionReqMyBidding.OnTimeout(oArg);
        }
    }
}
