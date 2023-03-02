﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 15:15:54

namespace XMainClient
{
	using ArgDataType = KKSG.FetchDGAppArg;
    using ResDataType = KKSG.FetchDGAppRes;

    class RpcC2M_FetchDGApps : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_FetchDGApps()
        {
        }

        public override uint GetRpcType()
        {
            return 48732;
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
            Process_RpcC2M_FetchDGApps.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FetchDGApps.OnTimeout(oArg);
        }
    }
}
