using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/9 11:48:54

namespace XMainClient
{
	using ArgDataType = KKSG.ActivatePreShowArg;
    using ResDataType = KKSG.ActivatePreShowRes;

    class RpcC2G_ActivatePreShow : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ActivatePreShow()
        {
        }

        public override uint GetRpcType()
        {
            return 22466;
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
            Process_RpcC2G_ActivatePreShow.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ActivatePreShow.OnTimeout(oArg);
        }
    }
}
