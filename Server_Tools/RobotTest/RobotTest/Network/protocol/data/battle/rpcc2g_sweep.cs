using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/13 14:28:32

namespace XMainClient
{
	using ArgDataType = KKSG.SweepArg;
    using ResDataType = KKSG.SweepRes;

    class RpcC2G_Sweep : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_Sweep()
        {
        }

        public override uint GetRpcType()
        {
            return 6019;
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
            Process_RpcC2G_Sweep.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_Sweep.OnTimeout(oArg);
        }
    }
}
