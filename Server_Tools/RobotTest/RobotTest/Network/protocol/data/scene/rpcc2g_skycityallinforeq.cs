using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 19:07:01

namespace XMainClient
{
	using ArgDataType = KKSG.SkyCityArg;
    using ResDataType = KKSG.SkyCityRes;

    class RpcC2G_SkyCityAllInfoReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SkyCityAllInfoReq()
        {
        }

        public override uint GetRpcType()
        {
            return 29365;
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
            Process_RpcC2G_SkyCityAllInfoReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SkyCityAllInfoReq.OnTimeout(oArg);
        }
    }
}
