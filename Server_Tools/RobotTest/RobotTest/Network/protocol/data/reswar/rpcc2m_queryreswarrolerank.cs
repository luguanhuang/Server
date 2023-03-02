using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/7 20:10:17

namespace XMainClient
{
	using ArgDataType = KKSG.ResWarRoleRankArg;
    using ResDataType = KKSG.ResWarRoleRankRes;

    class RpcC2M_QueryResWarRoleRank : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_QueryResWarRoleRank()
        {
        }

        public override uint GetRpcType()
        {
            return 27001;
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
            Process_RpcC2M_QueryResWarRoleRank.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_QueryResWarRoleRank.OnTimeout(oArg);
        }
    }
}
