﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/7 20:10:58

namespace XMainClient
{
	using ArgDataType = KKSG.LevelSealExchangeArg;
    using ResDataType = KKSG.LevelSealExchangeRes;

    class RpcC2G_LevelSealExchange : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_LevelSealExchange()
        {
        }

        public override uint GetRpcType()
        {
            return 65467;
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
            Process_RpcC2G_LevelSealExchange.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_LevelSealExchange.OnTimeout(oArg);
        }
    }
}
