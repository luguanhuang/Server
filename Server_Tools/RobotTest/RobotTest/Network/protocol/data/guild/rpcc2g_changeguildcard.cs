﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/2 10:43:56

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeGuildCardArg;
    using ResDataType = KKSG.ChangeGuildCardRes;

    class RpcC2G_ChangeGuildCard : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChangeGuildCard()
        {
        }

        public override uint GetRpcType()
        {
            return 55997;
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
            Process_RpcC2G_ChangeGuildCard.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeGuildCard.OnTimeout(oArg);
        }
    }
}
