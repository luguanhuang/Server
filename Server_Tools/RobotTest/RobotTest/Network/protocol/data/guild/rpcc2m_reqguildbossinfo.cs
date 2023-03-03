﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/5 16:23:13

namespace XMainClient
{
	using ArgDataType = KKSG.AskGuildBossInfoArg;
    using ResDataType = KKSG.AskGuildBossInfoRes;

    class RpcC2M_ReqGuildBossInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ReqGuildBossInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 38917;
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
            Process_RpcC2M_ReqGuildBossInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildBossInfo.OnTimeout(oArg);
        }
    }
}