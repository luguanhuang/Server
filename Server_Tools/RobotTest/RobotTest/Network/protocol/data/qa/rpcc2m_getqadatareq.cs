using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 16:27:30

namespace XMainClient
{
	using ArgDataType = KKSG.GetQADataReq;
    using ResDataType = KKSG.GetQADataRes;

    class RpcC2M_GetQADataReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetQADataReq()
        {
        }

        public override uint GetRpcType()
        {
            return 26871;
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
            Process_RpcC2M_GetQADataReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetQADataReq.OnTimeout(oArg);
        }
    }
}
