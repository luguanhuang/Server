using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/9 15:23:20

namespace XMainClient
{
	using ArgDataType = KKSG.OpenSceneChestArg;
    using ResDataType = KKSG.OpenSceneChestRes;

    class RpcC2G_OpenSceneChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_OpenSceneChest()
        {
        }

        public override uint GetRpcType()
        {
            return 27401;
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
            Process_RpcC2G_OpenSceneChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_OpenSceneChest.OnTimeout(oArg);
        }
    }
}
