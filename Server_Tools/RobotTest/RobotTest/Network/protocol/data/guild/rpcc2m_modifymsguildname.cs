using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 22:30:35

namespace XMainClient
{
	using ArgDataType = KKSG.ModifyArg;
    using ResDataType = KKSG.ModifyRes;

    class RpcC2M_ModifyMsGuildName : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ModifyMsGuildName()
        {
        }

        public override uint GetRpcType()
        {
            return 21709;
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
            Process_RpcC2M_ModifyMsGuildName.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ModifyMsGuildName.OnTimeout(oArg);
        }
    }
}
