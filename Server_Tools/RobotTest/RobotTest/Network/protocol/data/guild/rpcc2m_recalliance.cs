using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/20 11:11:05

namespace XMainClient
{
	using ArgDataType = KKSG.RecAllianceArg;
    using ResDataType = KKSG.RecAllianceRes;

    class RpcC2M_RecAlliance : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_RecAlliance()
        {
        }

        public override uint GetRpcType()
        {
            return 31937;
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
            Process_RpcC2M_RecAlliance.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_RecAlliance.OnTimeout(oArg);
        }
    }
}
