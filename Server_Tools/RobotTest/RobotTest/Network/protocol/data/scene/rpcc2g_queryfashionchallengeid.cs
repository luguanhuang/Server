using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/15 17:07:58

namespace XMainClient
{
	using ArgDataType = KKSG.QueryFashionArg;
    using ResDataType = KKSG.QueryFashionRes;

    class RpcC2G_QueryFashionChallengeID : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryFashionChallengeID()
        {
        }

        public override uint GetRpcType()
        {
            return 14165;
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
            Process_RpcC2G_QueryFashionChallengeID.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryFashionChallengeID.OnTimeout(oArg);
        }
    }
}
