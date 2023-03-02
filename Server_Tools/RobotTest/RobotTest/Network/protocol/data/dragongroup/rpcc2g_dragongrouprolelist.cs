using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/28 13:12:54

namespace XMainClient
{
	using ArgDataType = KKSG.DragonGroupRoleListC2S;
    using ResDataType = KKSG.DragonGroupRoleListS2C;

    class RpcC2G_DragonGroupRoleList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_DragonGroupRoleList()
        {
        }

        public override uint GetRpcType()
        {
            return 29660;
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
            Process_RpcC2G_DragonGroupRoleList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DragonGroupRoleList.OnTimeout(oArg);
        }
    }
}
