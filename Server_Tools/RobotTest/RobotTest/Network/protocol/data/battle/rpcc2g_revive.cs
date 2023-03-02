using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/14 15:33:55

namespace XMainClient
{
	using ArgDataType = KKSG.ReviveArg;
    using ResDataType = KKSG.ReviveRes;

    class RpcC2G_Revive : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_Revive()
        {
        }

        public override uint GetRpcType()
        {
            return 29831;
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
            Process_RpcC2G_Revive.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_Revive.OnTimeout(oArg);
        }
    }
}
