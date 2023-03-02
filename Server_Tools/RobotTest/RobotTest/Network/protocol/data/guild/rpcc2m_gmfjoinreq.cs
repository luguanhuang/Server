using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 19:14:35

namespace XMainClient
{
	using ArgDataType = KKSG.gmfjoinarg;
    using ResDataType = KKSG.gmfjoinres;

    class RpcC2M_gmfjoinreq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_gmfjoinreq()
        {
        }

        public override uint GetRpcType()
        {
            return 37651;
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
            Process_RpcC2M_gmfjoinreq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_gmfjoinreq.OnTimeout(oArg);
        }
    }
}
