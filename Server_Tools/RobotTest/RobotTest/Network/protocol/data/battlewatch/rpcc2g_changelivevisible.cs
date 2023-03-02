using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/10 14:35:46

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeLiveVisibleArg;
    using ResDataType = KKSG.ChangeLiveVisibleRes;

    class RpcC2G_ChangeLiveVisible : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ChangeLiveVisible()
        {
        }

        public override uint GetRpcType()
        {
            return 56831;
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
            Process_RpcC2G_ChangeLiveVisible.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeLiveVisible.OnTimeout(oArg);
        }
    }
}
