using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/9 14:33:17

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildQADataReq;
    using ResDataType = KKSG.GetGuildQADataRes;

    class RpcC2G_GetGuildQADataReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGuildQADataReq()
        {
        }

        public override uint GetRpcType()
        {
            return 35568;
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
            Process_RpcC2G_GetGuildQADataReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildQADataReq.OnTimeout(oArg);
        }
    }
}
