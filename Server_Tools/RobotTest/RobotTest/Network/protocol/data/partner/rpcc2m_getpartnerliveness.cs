using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 20:15:56

namespace XMainClient
{
	using ArgDataType = KKSG.GetPartnerLivenessArg;
    using ResDataType = KKSG.GetPartnerLivenessRes;

    class RpcC2M_GetPartnerLiveness : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetPartnerLiveness()
        {
        }

        public override uint GetRpcType()
        {
            return 18784;
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
            Process_RpcC2M_GetPartnerLiveness.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetPartnerLiveness.OnTimeout(oArg);
        }
    }
}
