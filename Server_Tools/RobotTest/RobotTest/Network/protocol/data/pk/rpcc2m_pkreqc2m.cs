using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/25 20:14:43

namespace XMainClient
{
	using ArgDataType = KKSG.PkReqArg;
    using ResDataType = KKSG.PkReqRes;

    class RpcC2M_PkReqC2M : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_PkReqC2M()
        {
        }

        public override uint GetRpcType()
        {
            return 41221;
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
            Process_RpcC2M_PkReqC2M.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_PkReqC2M.OnTimeout(oArg);
        }
    }
}
