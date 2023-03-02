using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/14 15:20:24

namespace XMainClient
{
	using ArgDataType = KKSG.JoinLeagueEleBattleArg;
    using ResDataType = KKSG.JoinLeagueEleBattleRes;

    class RpcC2M_JoinLeagueEleBattle : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_JoinLeagueEleBattle()
        {
        }

        public override uint GetRpcType()
        {
            return 43053;
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
            Process_RpcC2M_JoinLeagueEleBattle.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_JoinLeagueEleBattle.OnTimeout(oArg);
        }
    }
}
