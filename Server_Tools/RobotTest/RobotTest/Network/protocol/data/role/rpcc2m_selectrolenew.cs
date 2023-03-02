using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/10 23:52:57

namespace XMainClient
{
	using ArgDataType = KKSG.SelectRoleNewArg;
    using ResDataType = KKSG.SelectRoleNewRes;

    class RpcC2M_SelectRoleNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_SelectRoleNew()
        {
        }

        public override uint GetRpcType()
        {
            return 217;
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
            Process_RpcC2M_SelectRoleNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_SelectRoleNew.OnTimeout(oArg);
        }
    }
}
