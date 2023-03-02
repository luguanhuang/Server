using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/24 20:58:25

namespace XMainClient
{
	using ArgDataType = KKSG.FirstPassGetTopRoleInfoArg;
    using ResDataType = KKSG.FirstPassGetTopRoleInfoRes;

    class RpcC2G_FirstPassGetTopRoleInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FirstPassGetTopRoleInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 37076;
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
            Process_RpcC2G_FirstPassGetTopRoleInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FirstPassGetTopRoleInfo.OnTimeout(oArg);
        }
    }
}
