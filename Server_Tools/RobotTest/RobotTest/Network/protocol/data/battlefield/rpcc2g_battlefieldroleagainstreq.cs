﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/5 15:12:33

namespace XMainClient
{
	using ArgDataType = KKSG.BattleFieldRoleAgainstArg;
    using ResDataType = KKSG.BattleFieldRoleAgainst;

    class RpcC2G_BattleFieldRoleAgainstReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BattleFieldRoleAgainstReq()
        {
        }

        public override uint GetRpcType()
        {
            return 12475;
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
            Process_RpcC2G_BattleFieldRoleAgainstReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BattleFieldRoleAgainstReq.OnTimeout(oArg);
        }
    }
}
