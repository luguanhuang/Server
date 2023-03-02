using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/9 14:31:43

namespace XMainClient
{
	using ArgDataType = KKSG.OpenGuildQAReq;
    using ResDataType = KKSG.OpenGuildQARes;

    class RpcC2G_OpenGuildQAReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_OpenGuildQAReq()
        {
        }

        public override uint GetRpcType()
        {
            return 62840;
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
            Process_RpcC2G_OpenGuildQAReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_OpenGuildQAReq.OnTimeout(oArg);
        }
    }
}
