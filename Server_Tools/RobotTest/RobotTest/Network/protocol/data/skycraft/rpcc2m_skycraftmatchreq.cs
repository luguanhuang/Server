using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/1 11:56:04

namespace XMainClient
{
	using ArgDataType = KKSG.SkyCraftMatchReq;
    using ResDataType = KKSG.SkyCraftMatchRes;

    class RpcC2M_SkyCraftMatchReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_SkyCraftMatchReq()
        {
        }

        public override uint GetRpcType()
        {
            return 26016;
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
            Process_RpcC2M_SkyCraftMatchReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_SkyCraftMatchReq.OnTimeout(oArg);
        }
    }
}
