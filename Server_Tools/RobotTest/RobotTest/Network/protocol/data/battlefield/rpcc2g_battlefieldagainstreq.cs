using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/5 15:07:28

namespace XMainClient
{
	using ArgDataType = KKSG.BattleFieldRoleAgainstArg;
    using ResDataType = KKSG.BattleFieldRoleAgainst;

    class RpcC2G_BattleFieldAgainstReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BattleFieldAgainstReq()
        {
        }

        public override uint GetRpcType()
        {
            return 28863;
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
            Process_RpcC2G_BattleFieldAgainstReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BattleFieldAgainstReq.OnTimeout(oArg);
        }
    }
}
