using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/20 17:49:28

namespace XMainClient
{
	using ArgDataType = KKSG.SurviveReqArg;
    using ResDataType = KKSG.SurviveReqRes;

    class RpcC2G_SurviveReqC2G : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_SurviveReqC2G()
        {
        }

        public override uint GetRpcType()
        {
            return 19408;
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
            Process_RpcC2G_SurviveReqC2G.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SurviveReqC2G.OnTimeout(oArg);
        }
    }
}
