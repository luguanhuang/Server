using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/7 17:02:30

namespace XMainClient
{
	using ArgDataType = KKSG.SceneMobaOpArg;
    using ResDataType = KKSG.SceneMobaOpRes;

    class RpcC2G_SceneMobaOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_SceneMobaOp()
        {
        }

        public override uint GetRpcType()
        {
            return 32594;
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
            Process_RpcC2G_SceneMobaOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SceneMobaOp.OnTimeout(oArg);
        }
    }
}
