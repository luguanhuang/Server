using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/22 16:57:14

namespace XMainClient
{
	using ArgDataType = KKSG.WeekEndNestArg;
    using ResDataType = KKSG.WeekEndNestRes;

    class RpcC2G_WeekEndNest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_WeekEndNest()
        {
        }

        public override uint GetRpcType()
        {
            return 8058;
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
            Process_RpcC2G_WeekEndNest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_WeekEndNest.OnTimeout(oArg);
        }
    }
}
