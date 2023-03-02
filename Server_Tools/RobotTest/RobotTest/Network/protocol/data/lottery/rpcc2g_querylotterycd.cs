using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/15 10:02:04

namespace XMainClient
{
	using ArgDataType = KKSG.QueryLotteryCDArg;
    using ResDataType = KKSG.QueryLotteryCDRes;

    class RpcC2G_QueryLotteryCD : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_QueryLotteryCD()
        {
        }

        public override uint GetRpcType()
        {
            return 12242;
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
            Process_RpcC2G_QueryLotteryCD.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryLotteryCD.OnTimeout(oArg);
        }
    }
}
