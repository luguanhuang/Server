using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/17 17:06:08

namespace XMainClient
{
	using ArgDataType = KKSG.AceptGuildInheritArg;
    using ResDataType = KKSG.AceptGuildInheritRes;

    class RpcC2M_AceptGuildInherit : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_AceptGuildInherit()
        {
        }

        public override uint GetRpcType()
        {
            return 35235;
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
            Process_RpcC2M_AceptGuildInherit.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AceptGuildInherit.OnTimeout(oArg);
        }
    }
}
