using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/30 12:36:47

namespace XMainClient
{
	using ArgDataType = KKSG.ClickDesignationReq;
    using ResDataType = KKSG.ClickDesignationRes;

    class RpcC2G_ClickDesignationReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ClickDesignationReq()
        {
        }

        public override uint GetRpcType()
        {
            return 8916;
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
            Process_RpcC2G_ClickDesignationReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ClickDesignationReq.OnTimeout(oArg);
        }
    }
}
