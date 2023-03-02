﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/11 16:12:16

namespace XMainClient
{
	using ArgDataType = KKSG.GetMyWatchRecordArg;
    using ResDataType = KKSG.GetMyWatchRecordRes;

    class RpcC2G_GetMyWatchRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetMyWatchRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 22907;
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
            Process_RpcC2G_GetMyWatchRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetMyWatchRecord.OnTimeout(oArg);
        }
    }
}
