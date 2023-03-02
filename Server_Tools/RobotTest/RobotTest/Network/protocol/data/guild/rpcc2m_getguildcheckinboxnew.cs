using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/16 20:30:46

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildCheckinBoxArg;
    using ResDataType = KKSG.GetGuildCheckinBoxRes;

    class RpcC2M_GetGuildCheckinBoxNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetGuildCheckinBoxNew()
        {
        }

        public override uint GetRpcType()
        {
            return 28115;
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
            Process_RpcC2M_GetGuildCheckinBoxNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetGuildCheckinBoxNew.OnTimeout(oArg);
        }
    }
}
