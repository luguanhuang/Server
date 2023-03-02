﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/25 16:40:12

namespace XMainClient
{
	using ArgDataType = KKSG.GetTowerActivityTopArg;
    using ResDataType = KKSG.GetTowerActivityTopRes;

    class RpcC2G_GetTowerActivityTop : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetTowerActivityTop()
        {
        }

        public override uint GetRpcType()
        {
            return 5168;
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
            Process_RpcC2G_GetTowerActivityTop.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetTowerActivityTop.OnTimeout(oArg);
        }
    }
}
