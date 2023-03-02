using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/9 15:39:25

namespace XMainClient
{
	using ArgDataType = KKSG.TShowTopListArg;
    using ResDataType = KKSG.TShowTopListRes;

    class RpcC2G_TShowTopList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TShowTopList()
        {
        }

        public override uint GetRpcType()
        {
            return 19997;
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
            Process_RpcC2G_TShowTopList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TShowTopList.OnTimeout(oArg);
        }
    }
}
