using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/16 21:06:53

namespace XMainClient
{
	using ArgDataType = KKSG.EndFishingArg;
    using ResDataType = KKSG.EndFishingRes;

    class RpcC2G_EndFishing : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_EndFishing()
        {
        }

        public override uint GetRpcType()
        {
            return 28402;
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
            Process_RpcC2G_EndFishing.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EndFishing.OnTimeout(oArg);
        }
    }
}
