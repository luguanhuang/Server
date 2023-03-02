using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/10 11:10:30

namespace XMainClient
{
	using ArgDataType = KKSG.AddPkReq;
    using ResDataType = KKSG.AddPkRes;

    class RpcC2G_AddPk : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AddPk()
        {
        }

        public override uint GetRpcType()
        {
            return 61829;
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
            Process_RpcC2G_AddPk.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AddPk.OnTimeout(oArg);
        }
    }
}
