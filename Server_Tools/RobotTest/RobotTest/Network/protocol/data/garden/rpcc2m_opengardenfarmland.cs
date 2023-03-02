﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/26 21:31:37

namespace XMainClient
{
	using ArgDataType = KKSG.OpenGardenFarmlandArg;
    using ResDataType = KKSG.OpenGardenFarmlandRes;

    class RpcC2M_OPenGardenFarmland : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_OPenGardenFarmland()
        {
        }

        public override uint GetRpcType()
        {
            return 42589;
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
            Process_RpcC2M_OPenGardenFarmland.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_OPenGardenFarmland.OnTimeout(oArg);
        }
    }
}
