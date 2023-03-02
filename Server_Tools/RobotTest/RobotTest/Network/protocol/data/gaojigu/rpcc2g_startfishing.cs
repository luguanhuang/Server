using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/16 20:57:53

namespace XMainClient
{
	using ArgDataType = KKSG.StartFishingArg;
    using ResDataType = KKSG.StartFishingRes;

    class RpcC2G_StartFishing : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_StartFishing()
        {
        }

        public override uint GetRpcType()
        {
            return 30057;
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
            Process_RpcC2G_StartFishing.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_StartFishing.OnTimeout(oArg);
        }
    }
}
