using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/11 16:33:03

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeProfessionArg;
    using ResDataType = KKSG.ChangeProfessionRes;

    class RpcC2G_ChangeProfession : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ChangeProfession()
        {
        }

        public override uint GetRpcType()
        {
            return 48822;
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
            Process_RpcC2G_ChangeProfession.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeProfession.OnTimeout(oArg);
        }
    }
}
