using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/19 18:30:49

namespace XMainClient
{
	using ArgDataType = KKSG.AdjustGuildArenaRolePosArg;
    using ResDataType = KKSG.AdjustGuildArenaRolePosRes;

    class RpcC2G_AdjustGuildArenaRolePos : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AdjustGuildArenaRolePos()
        {
        }

        public override uint GetRpcType()
        {
            return 65149;
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
            Process_RpcC2G_AdjustGuildArenaRolePos.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AdjustGuildArenaRolePos.OnTimeout(oArg);
        }
    }
}
