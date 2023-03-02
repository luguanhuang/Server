using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/11 17:35:50

namespace XMainClient
{
	using ArgDataType = KKSG.AuctROSPara;
    using ResDataType = KKSG.AuctionReqResult;

    class RpcC2G_AuctionReqOutSale : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AuctionReqOutSale()
        {
        }

        public override uint GetRpcType()
        {
            return 22560;
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
            Process_RpcC2G_AuctionReqOutSale.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AuctionReqOutSale.OnTimeout(oArg);
        }
    }
}
