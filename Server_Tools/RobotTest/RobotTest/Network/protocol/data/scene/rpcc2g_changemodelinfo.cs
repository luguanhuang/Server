using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/16 21:07:46

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeModelInfoArg;
    using ResDataType = KKSG.ChangeModelInfoRes;

    class RpcC2G_ChangeModelInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChangeModelInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 13085;
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
            Process_RpcC2G_ChangeModelInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeModelInfo.OnTimeout(oArg);
        }
    }
}
