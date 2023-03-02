using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/12 15:50:23

namespace XMainClient
{
	using ArgDataType = KKSG.TShowVoteArg;
    using ResDataType = KKSG.TShowVoteRes;

    class RpcC2G_TShowVote : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TShowVote()
        {
        }

        public override uint GetRpcType()
        {
            return 12292;
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
            Process_RpcC2G_TShowVote.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TShowVote.OnTimeout(oArg);
        }
    }
}
