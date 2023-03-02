using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/31 14:24:18

namespace XMainClient
{
	using ArgDataType = KKSG.GaoJiGuCancelModelArg;
    using ResDataType = KKSG.GaoJiGuCancelModelRes;

    class RpcC2G_GaoJiGuCancelModel : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GaoJiGuCancelModel()
        {
        }

        public override uint GetRpcType()
        {
            return 41936;
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
            Process_RpcC2G_GaoJiGuCancelModel.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GaoJiGuCancelModel.OnTimeout(oArg);
        }
    }
}
