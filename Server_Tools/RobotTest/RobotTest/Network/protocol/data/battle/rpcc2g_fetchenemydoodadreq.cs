using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/25 15:24:21

namespace XMainClient
{
	using ArgDataType = KKSG.EnemyDoodadInfo;
    using ResDataType = KKSG.RollInfoRes;

    class RpcC2G_FetchEnemyDoodadReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FetchEnemyDoodadReq()
        {
        }

        public override uint GetRpcType()
        {
            return 56348;
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
            Process_RpcC2G_FetchEnemyDoodadReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FetchEnemyDoodadReq.OnTimeout(oArg);
        }
    }
}
