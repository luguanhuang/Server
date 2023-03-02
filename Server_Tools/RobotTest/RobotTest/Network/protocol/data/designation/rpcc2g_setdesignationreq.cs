using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/30 12:30:23

namespace XMainClient
{
	using ArgDataType = KKSG.SetDesignationReq;
    using ResDataType = KKSG.SetDesignationRes;

    class RpcC2G_SetDesignationReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SetDesignationReq()
        {
        }

        public override uint GetRpcType()
        {
            return 7673;
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
            Process_RpcC2G_SetDesignationReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SetDesignationReq.OnTimeout(oArg);
        }
    }
}
