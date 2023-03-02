using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/2 23:01:02

namespace XMainClient
{
	using ArgDataType = KKSG.QueryResWarArg;
    using ResDataType = KKSG.QueryResWarRes;

    class RpcC2M_QueryResWar : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_QueryResWar()
        {
        }

        public override uint GetRpcType()
        {
            return 41509;
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
            Process_RpcC2M_QueryResWar.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_QueryResWar.OnTimeout(oArg);
        }
    }
}
