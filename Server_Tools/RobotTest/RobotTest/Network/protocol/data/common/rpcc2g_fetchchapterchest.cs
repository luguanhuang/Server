using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 22:23:11

namespace XMainClient
{
	using ArgDataType = KKSG.FetchChapterChestArg;
    using ResDataType = KKSG.FetchChapterChestRes;

    class RpcC2G_FetchChapterChest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FetchChapterChest()
        {
        }

        public override uint GetRpcType()
        {
            return 21099;
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
            Process_RpcC2G_FetchChapterChest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FetchChapterChest.OnTimeout(oArg);
        }
    }
}
