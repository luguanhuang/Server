using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/5 16:18:08

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeRiskBoxStateArg;
    using ResDataType = KKSG.ChangeRiskBoxStateRes;

    class RpcC2G_ChangeRiskBoxState : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChangeRiskBoxState()
        {
        }

        public override uint GetRpcType()
        {
            return 4472;
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
            Process_RpcC2G_ChangeRiskBoxState.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeRiskBoxState.OnTimeout(oArg);
        }
    }
}
