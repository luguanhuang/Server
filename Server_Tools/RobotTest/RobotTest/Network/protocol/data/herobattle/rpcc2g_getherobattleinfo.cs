﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/13 16:36:04

namespace XMainClient
{
	using ArgDataType = KKSG.GetHeroBattleInfoArg;
    using ResDataType = KKSG.GetHeroBattleInfoRes;

    class RpcC2G_GetHeroBattleInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetHeroBattleInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 65206;
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
            Process_RpcC2G_GetHeroBattleInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetHeroBattleInfo.OnTimeout(oArg);
        }
    }
}
