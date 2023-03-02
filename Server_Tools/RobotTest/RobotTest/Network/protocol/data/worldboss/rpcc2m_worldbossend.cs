using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/3 21:08:42

namespace XMainClient
{
	using ArgDataType = KKSG.WorldBossEndArg;
    using ResDataType = KKSG.WorldBossEndRes;

    class RpcC2M_WorldBossEnd : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_WorldBossEnd()
        {
        }

        public override uint GetRpcType()
        {
            return 53655;
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
            Process_RpcC2M_WorldBossEnd.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_WorldBossEnd.OnTimeout(oArg);
        }
    }
}
