using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/26 14:31:51

namespace XMainClient
{
	using ArgDataType = KKSG.GetMobaBattleInfoArg;
    using ResDataType = KKSG.GetMobaBattleInfoRes;

    class RpcC2M_GetMobaBattleInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMobaBattleInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 64051;
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
            Process_RpcC2M_GetMobaBattleInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMobaBattleInfo.OnTimeout(oArg);
        }
    }
}
