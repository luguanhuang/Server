using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/15 16:22:03

namespace XMainClient
{
	using ArgDataType = KKSG.FetchTeamListArg;
    using ResDataType = KKSG.FetchTeamListRes;

    class RpcC2G_FetchTeamList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FetchTeamList()
        {
        }

        public override uint GetRpcType()
        {
            return 15911;
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
            Process_RpcC2G_FetchTeamList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FetchTeamList.OnTimeout(oArg);
        }
    }
}
