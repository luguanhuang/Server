using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/28 15:59:07

namespace XMainClient
{
	using ArgDataType = KKSG.GetMarriageLivenessArg;
    using ResDataType = KKSG.GetMarriageLivenessRes;

    class RpcC2M_GetMarriageLiveness : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMarriageLiveness()
        {
        }

        public override uint GetRpcType()
        {
            return 30055;
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
            Process_RpcC2M_GetMarriageLiveness.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMarriageLiveness.OnTimeout(oArg);
        }
    }
}
