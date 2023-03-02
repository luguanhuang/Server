using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/10 17:52:15

namespace XMainClient
{
	using ArgDataType = KKSG.FetchPlatNoticeArg;
    using ResDataType = KKSG.FetchPlatNoticeRes;

    class RpcC2M_FetchPlatNotice : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_FetchPlatNotice()
        {
        }

        public override uint GetRpcType()
        {
            return 60271;
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
            Process_RpcC2M_FetchPlatNotice.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_FetchPlatNotice.OnTimeout(oArg);
        }
    }
}
