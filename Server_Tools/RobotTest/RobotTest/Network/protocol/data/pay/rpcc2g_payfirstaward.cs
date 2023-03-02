﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/22 20:34:06

namespace XMainClient
{
	using ArgDataType = KKSG.PayFirstAwardArg;
    using ResDataType = KKSG.PayFirstAwardRes;

    class RpcC2G_PayFirstAward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_PayFirstAward()
        {
        }

        public override uint GetRpcType()
        {
            return 46058;
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
            Process_RpcC2G_PayFirstAward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PayFirstAward.OnTimeout(oArg);
        }
    }
}
