using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/21 14:42:06

namespace XMainClient
{
	using ArgDataType = KKSG.AddBlackListArg;
    using ResDataType = KKSG.AddBlackListRes;

    class RpcC2G_AddBlackList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AddBlackList()
        {
        }

        public override uint GetRpcType()
        {
            return 16981;
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
            Process_RpcC2G_AddBlackList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AddBlackList.OnTimeout(oArg);
        }
    }
}
