using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/18 16:31:32

namespace XMainClient
{
	using ArgDataType = KKSG.DelGuildInheritArg;
    using ResDataType = KKSG.DelGuildInheritRes;

    class RpcC2M_DelGuildInherit : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_DelGuildInherit()
        {
        }

        public override uint GetRpcType()
        {
            return 3671;
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
            Process_RpcC2M_DelGuildInherit.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DelGuildInherit.OnTimeout(oArg);
        }
    }
}
