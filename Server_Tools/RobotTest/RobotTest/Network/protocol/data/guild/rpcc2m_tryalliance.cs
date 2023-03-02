using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/20 11:19:14

namespace XMainClient
{
	using ArgDataType = KKSG.TryAllianceArg;
    using ResDataType = KKSG.TryAlliance;

    class RpcC2M_TryAlliance : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_TryAlliance()
        {
        }

        public override uint GetRpcType()
        {
            return 20216;
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
            Process_RpcC2M_TryAlliance.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_TryAlliance.OnTimeout(oArg);
        }
    }
}
