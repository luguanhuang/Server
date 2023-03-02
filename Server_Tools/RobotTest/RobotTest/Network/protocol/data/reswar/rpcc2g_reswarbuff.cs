using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/26 17:31:00

namespace XMainClient
{
	using ArgDataType = KKSG.ResWarBuffArg;
    using ResDataType = KKSG.ResWarBuffRes;

    class RpcC2G_ResWarBuff : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ResWarBuff()
        {
        }

        public override uint GetRpcType()
        {
            return 17670;
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
            Process_RpcC2G_ResWarBuff.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ResWarBuff.OnTimeout(oArg);
        }
    }
}
