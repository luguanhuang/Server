using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/29 20:29:34

namespace XMainClient
{
	using ArgDataType = KKSG.ArenaStarReqArg;
    using ResDataType = KKSG.ArenaStarReqRes;

    class RpcC2M_ArenaStarRoleReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ArenaStarRoleReq()
        {
        }

        public override uint GetRpcType()
        {
            return 53598;
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
            Process_RpcC2M_ArenaStarRoleReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ArenaStarRoleReq.OnTimeout(oArg);
        }
    }
}
