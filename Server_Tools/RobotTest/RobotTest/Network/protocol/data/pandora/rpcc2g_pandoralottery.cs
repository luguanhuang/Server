using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/17 12:18:05

namespace XMainClient
{
	using ArgDataType = KKSG.PandoraLotteryArg;
    using ResDataType = KKSG.PandoraLotteryRes;

    class RpcC2G_PandoraLottery : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_PandoraLottery()
        {
        }

        public override uint GetRpcType()
        {
            return 12575;
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
            Process_RpcC2G_PandoraLottery.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PandoraLottery.OnTimeout(oArg);
        }
    }
}
