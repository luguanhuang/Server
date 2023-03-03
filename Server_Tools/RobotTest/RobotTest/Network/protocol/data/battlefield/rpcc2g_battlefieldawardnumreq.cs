﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/4 14:06:51

namespace XMainClient
{
	using ArgDataType = KKSG.BattleFieldAwardNumArg;
    using ResDataType = KKSG.BattleFieldAwardNumRes;

    class RpcC2G_BattleFieldAwardNumReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BattleFieldAwardNumReq()
        {
        }

        public override uint GetRpcType()
        {
            return 59171;
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
            Process_RpcC2G_BattleFieldAwardNumReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BattleFieldAwardNumReq.OnTimeout(oArg);
        }
    }
}