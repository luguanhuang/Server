using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/5/27 13:53:38

namespace XMainClient
{
	using ArgDataType = KKSG.GetRoleArenaInfoArg;
    using ResDataType = KKSG.GetRoleArenaInfoRes;

    class RpcC2G_GetRoleArenaInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetRoleArenaInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 52929;
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
            Process_RpcC2G_GetRoleArenaInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetRoleArenaInfo.OnTimeout(oArg);
        }
    }
}
