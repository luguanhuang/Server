using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 14:12:43

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildCheckinBoxArg;
    using ResDataType = KKSG.GetGuildCheckinBoxRes;

    class RpcC2G_GetGuildCheckinBox : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGuildCheckinBox()
        {
        }

        public override uint GetRpcType()
        {
            return 19269;
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
            Process_RpcC2G_GetGuildCheckinBox.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildCheckinBox.OnTimeout(oArg);
        }
    }
}
