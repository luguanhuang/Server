using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/20 10:29:26

namespace XMainClient
{
	using ArgDataType = KKSG.GCFFightInfoArg;
    using ResDataType = KKSG.GCFFightInfoRes;

    class RpcC2M_GCFFightInfoReqC2M : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GCFFightInfoReqC2M()
        {
        }

        public override uint GetRpcType()
        {
            return 42852;
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
            Process_RpcC2M_GCFFightInfoReqC2M.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GCFFightInfoReqC2M.OnTimeout(oArg);
        }
    }
}
