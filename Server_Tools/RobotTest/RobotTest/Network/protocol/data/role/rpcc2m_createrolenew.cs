using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/10 23:53:37

namespace XMainClient
{
	using ArgDataType = KKSG.CreateRoleNewArg;
    using ResDataType = KKSG.CreateRoleNewRes;

    class RpcC2M_CreateRoleNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_CreateRoleNew()
        {
        }

        public override uint GetRpcType()
        {
            return 13034;
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
            Process_RpcC2M_CreateRoleNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_CreateRoleNew.OnTimeout(oArg);
        }
    }
}
