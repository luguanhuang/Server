using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/5 20:47:19

namespace XMainClient
{
	using ArgDataType = KKSG.LeaveLeagueTeamArg;
    using ResDataType = KKSG.LeaveLeagueTeamRes;

    class RpcC2M_LeaveLeagueTeam : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_LeaveLeagueTeam()
        {
        }

        public override uint GetRpcType()
        {
            return 47239;
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
            Process_RpcC2M_LeaveLeagueTeam.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_LeaveLeagueTeam.OnTimeout(oArg);
        }
    }
}
