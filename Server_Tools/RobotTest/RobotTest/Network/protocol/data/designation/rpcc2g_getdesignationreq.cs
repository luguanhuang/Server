using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/1 20:59:59

namespace XMainClient
{
	using ArgDataType = KKSG.GetDesignationReq;
    using ResDataType = KKSG.GetDesignationRes;

    class RpcC2G_GetDesignationReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetDesignationReq()
        {
        }

        public override uint GetRpcType()
        {
            return 44412;
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
            Process_RpcC2G_GetDesignationReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetDesignationReq.OnTimeout(oArg);
        }
    }
}
