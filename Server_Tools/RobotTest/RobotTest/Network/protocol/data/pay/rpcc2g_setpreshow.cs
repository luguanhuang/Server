using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 15:32:38

namespace XMainClient
{
	using ArgDataType = KKSG.SetPreShowArg;
    using ResDataType = KKSG.SetPreShowRes;

    class RpcC2G_SetPreShow : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_SetPreShow()
        {
        }

        public override uint GetRpcType()
        {
            return 346;
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
            Process_RpcC2G_SetPreShow.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SetPreShow.OnTimeout(oArg);
        }
    }
}
