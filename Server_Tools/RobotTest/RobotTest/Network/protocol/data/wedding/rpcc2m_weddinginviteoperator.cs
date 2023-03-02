using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/25 9:58:08

namespace XMainClient
{
	using ArgDataType = KKSG.WeddingInviteOperatorArg;
    using ResDataType = KKSG.WeddingInviteOperatorRes;

    class RpcC2M_WeddingInviteOperator : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_WeddingInviteOperator()
        {
        }

        public override uint GetRpcType()
        {
            return 8562;
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
            Process_RpcC2M_WeddingInviteOperator.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_WeddingInviteOperator.OnTimeout(oArg);
        }
    }
}
