using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/20 17:51:08

namespace XMainClient
{
	using ArgDataType = KKSG.ReportBattleArg;
    using ResDataType = KKSG.ReportBattleRes;

    class RpcC2G_ReportBattle : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ReportBattle()
        {
            oArg.battledata = new KKSG.BattleData();
        }

        public override uint GetRpcType()
        {
            return 21292;
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
            Process_RpcC2G_ReportBattle.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReportBattle.OnTimeout(oArg);
        }
    }
}
