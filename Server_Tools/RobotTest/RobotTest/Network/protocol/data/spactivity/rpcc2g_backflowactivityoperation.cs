using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/19 11:11:20

namespace XMainClient
{
	using ArgDataType = KKSG.BackFlowActivityOperationArg;
    using ResDataType = KKSG.BackFlowActivityOperationRes;

    class RpcC2G_BackFlowActivityOperation : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BackFlowActivityOperation()
        {
        }

        public override uint GetRpcType()
        {
            return 61579;
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
            Process_RpcC2G_BackFlowActivityOperation.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BackFlowActivityOperation.OnTimeout(oArg);
        }
    }
}
