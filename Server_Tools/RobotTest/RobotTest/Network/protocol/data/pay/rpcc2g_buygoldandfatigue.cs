using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/20 10:15:31

namespace XMainClient
{
	using ArgDataType = KKSG.BuyGoldAndFatigueArg;
    using ResDataType = KKSG.BuyGoldAndFatigueRes;

    class RpcC2G_BuyGoldAndFatigue : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_BuyGoldAndFatigue()
        {
        }

        public override uint GetRpcType()
        {
            return 31095;
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
            Process_RpcC2G_BuyGoldAndFatigue.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BuyGoldAndFatigue.OnTimeout(oArg);
        }
    }
}
