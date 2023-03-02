using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/22 16:09:14

namespace XMainClient
{
	using ArgDataType = KKSG.TShowTopListArg;
    using ResDataType = KKSG.TShowTopListRes;

    class RpcC2M_MTShowTopList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_MTShowTopList()
        {
        }

        public override uint GetRpcType()
        {
            return 10166;
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
            Process_RpcC2M_MTShowTopList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_MTShowTopList.OnTimeout(oArg);
        }
    }
}
