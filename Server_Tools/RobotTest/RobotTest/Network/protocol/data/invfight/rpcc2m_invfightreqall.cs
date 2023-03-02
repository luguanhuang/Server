using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/13 23:25:17

namespace XMainClient
{
	using ArgDataType = KKSG.InvFightArg;
    using ResDataType = KKSG.InvFightRes;

    class RpcC2M_InvFightReqAll : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_InvFightReqAll()
        {
        }

        public override uint GetRpcType()
        {
            return 56726;
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
            Process_RpcC2M_InvFightReqAll.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_InvFightReqAll.OnTimeout(oArg);
        }
    }
}
