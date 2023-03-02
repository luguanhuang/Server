using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/14 16:36:10

namespace XMainClient
{
	using ArgDataType = KKSG.QueryOpenGameArg;
    using ResDataType = KKSG.QueryOpenGameRes;

    class RpcC2G_QueryOpenGameActivityTime : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryOpenGameActivityTime()
        {
        }

        public override uint GetRpcType()
        {
            return 24079;
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
            Process_RpcC2G_QueryOpenGameActivityTime.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryOpenGameActivityTime.OnTimeout(oArg);
        }
    }
}
