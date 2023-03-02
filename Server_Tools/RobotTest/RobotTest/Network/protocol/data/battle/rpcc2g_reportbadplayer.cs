using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/10 16:10:32

namespace XMainClient
{
	using ArgDataType = KKSG.ReportBadPlayerArg;
    using ResDataType = KKSG.ReportBadPlayerRes;

    class RpcC2G_ReportBadPlayer : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ReportBadPlayer()
        {
        }

        public override uint GetRpcType()
        {
            return 32807;
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
            Process_RpcC2G_ReportBadPlayer.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReportBadPlayer.OnTimeout(oArg);
        }
    }
}
