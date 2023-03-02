using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/5 16:21:07

namespace XMainClient
{
	using ArgDataType = KKSG.ReEnterRiskBattleArg;
    using ResDataType = KKSG.ReEnterRiskBattleRes;

    class RpcC2G_ReEnterRiskBattle : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ReEnterRiskBattle()
        {
        }

        public override uint GetRpcType()
        {
            return 1615;
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
            Process_RpcC2G_ReEnterRiskBattle.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReEnterRiskBattle.OnTimeout(oArg);
        }
    }
}
