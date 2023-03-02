using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/17 15:39:53

namespace XMainClient
{
	using ArgDataType = KKSG.AddGuildInheritArg;
    using ResDataType = KKSG.AddGuildInheritRes;

    class RpcC2M_AddGuildInherit : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AddGuildInherit()
        {
        }

        public override uint GetRpcType()
        {
            return 15845;
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
            Process_RpcC2M_AddGuildInherit.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AddGuildInherit.OnTimeout(oArg);
        }
    }
}
