﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/16 11:00:33

namespace XMainClient
{
	using ArgDataType = KKSG.GetMobaBattleBriefRecordArg;
    using ResDataType = KKSG.GetMobaBattleBriefRecordRes;

    class RpcC2M_GetMobaBattleBriefRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMobaBattleBriefRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 35507;
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
            Process_RpcC2M_GetMobaBattleBriefRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMobaBattleBriefRecord.OnTimeout(oArg);
        }
    }
}
