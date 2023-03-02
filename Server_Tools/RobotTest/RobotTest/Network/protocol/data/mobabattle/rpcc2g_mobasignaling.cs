using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/13 19:39:42

namespace XMainClient
{
	using ArgDataType = KKSG.MobaSignalingArg;
    using ResDataType = KKSG.MobaSignalingRes;

    class RpcC2G_MobaSignaling : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_MobaSignaling()
        {
        }

        public override uint GetRpcType()
        {
            return 52475;
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
            Process_RpcC2G_MobaSignaling.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_MobaSignaling.OnTimeout(oArg);
        }
    }
}
