﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/14 19:56:29

namespace XMainClient
{
	using ArgDataType = KKSG.GetDailyTaskInfoArg;
    using ResDataType = KKSG.GetDailyTaskInfoRes;

    class RpcC2G_GetDailyTaskInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetDailyTaskInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 52480;
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
            Process_RpcC2G_GetDailyTaskInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetDailyTaskInfo.OnTimeout(oArg);
        }
    }
}
