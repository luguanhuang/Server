using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/4 15:25:09

namespace XMainClient
{
	using ArgDataType = KKSG.GetLeagueTeamInfoArg;
    using ResDataType = KKSG.GetLeagueTeamInfoRes;

    class RpcC2M_GetLeagueTeamInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetLeagueTeamInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 12488;
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
            Process_RpcC2M_GetLeagueTeamInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetLeagueTeamInfo.OnTimeout(oArg);
        }
    }
}
