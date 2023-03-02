using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/19 14:04:22

namespace XMainClient
{
	using ArgDataType = KKSG.GetNewestVersionArg;
    using ResDataType = KKSG.GetNewestVersionRes;

    class RpcC2I_GetNewestVersion : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2I_GetNewestVersion()
        {
        }

        public override uint GetRpcType()
        {
            return 47779;
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
            Process_RpcC2I_GetNewestVersion.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2I_GetNewestVersion.OnTimeout(oArg);
        }
    }
}
