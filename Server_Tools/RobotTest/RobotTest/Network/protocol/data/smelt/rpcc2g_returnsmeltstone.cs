﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/13 19:38:04

namespace XMainClient
{
	using ArgDataType = KKSG.ReturnSmeltStoneArg;
    using ResDataType = KKSG.ReturnSmeltStoneRes;

    class RpcC2G_ReturnSmeltStone : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ReturnSmeltStone()
        {
        }

        public override uint GetRpcType()
        {
            return 16978;
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
            Process_RpcC2G_ReturnSmeltStone.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ReturnSmeltStone.OnTimeout(oArg);
        }
    }
}
