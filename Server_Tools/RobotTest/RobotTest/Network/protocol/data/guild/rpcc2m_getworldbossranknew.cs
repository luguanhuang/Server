using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/15 16:21:02

namespace XMainClient
{
	using ArgDataType = KKSG.GetWorldBossRankArg;
    using ResDataType = KKSG.GetWorldBossRankRes;

    class RpcC2M_getworldbossranknew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_getworldbossranknew()
        {
        }

        public override uint GetRpcType()
        {
            return 47210;
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
            Process_RpcC2M_getworldbossranknew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_getworldbossranknew.OnTimeout(oArg);
        }
    }
}
