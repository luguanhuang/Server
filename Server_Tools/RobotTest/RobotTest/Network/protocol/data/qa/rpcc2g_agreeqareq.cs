using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/6 21:15:29

namespace XMainClient
{
	using ArgDataType = KKSG.AgreeQAReq;
    using ResDataType = KKSG.AgreeQARes;

    class RpcC2G_AgreeQAReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AgreeQAReq()
        {
        }

        public override uint GetRpcType()
        {
            return 43200;
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
            Process_RpcC2G_AgreeQAReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AgreeQAReq.OnTimeout(oArg);
        }
    }
}
