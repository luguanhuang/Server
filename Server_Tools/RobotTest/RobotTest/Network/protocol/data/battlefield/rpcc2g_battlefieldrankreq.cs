using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/4 15:27:16

namespace XMainClient
{
	using ArgDataType = KKSG.BattleFieldRankArg;
    using ResDataType = KKSG.BattleFieldRankRes;

    class RpcC2G_BattleFieldRankReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BattleFieldRankReq()
        {
        }

        public override uint GetRpcType()
        {
            return 4893;
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
            Process_RpcC2G_BattleFieldRankReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BattleFieldRankReq.OnTimeout(oArg);
        }
    }
}
