using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 16:26:35

namespace XMainClient
{
	using ArgDataType = KKSG.GetPlatformShareChestArg;
    using ResDataType = KKSG.GetPlatformShareChestRes;

    class RpcC2G_GetPlatformShareChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetPlatformShareChest()
        {
        }

        public override uint GetRpcType()
        {
            return 7875;
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
            Process_RpcC2G_GetPlatformShareChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetPlatformShareChest.OnTimeout(oArg);
        }
    }
}
