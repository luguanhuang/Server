using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/12 16:03:46

namespace XMainClient
{
	using ArgDataType = KKSG.TShowVoteInfoArg;
    using ResDataType = KKSG.TShowVoteInfoRes;

    class RpcC2G_TShowVoteInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TShowVoteInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 18365;
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
            Process_RpcC2G_TShowVoteInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TShowVoteInfo.OnTimeout(oArg);
        }
    }
}
