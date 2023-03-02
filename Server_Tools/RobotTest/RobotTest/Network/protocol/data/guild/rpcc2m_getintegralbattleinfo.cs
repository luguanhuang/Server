using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/3 20:52:29

namespace XMainClient
{
	using ArgDataType = KKSG.getintegralbattleInfoarg;
    using ResDataType = KKSG.getintegralbattleInfores;

    class RpcC2M_getintegralbattleInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_getintegralbattleInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 27825;
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
            Process_RpcC2M_getintegralbattleInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_getintegralbattleInfo.OnTimeout(oArg);
        }
    }
}
