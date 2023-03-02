using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/30 16:18:36

namespace XMainClient
{
	using ArgDataType = KKSG.ItemFindBackInfoArg;
    using ResDataType = KKSG.ItemFindBackInfoRes;

    class RpcC2G_ItemFindBackInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ItemFindBackInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 11755;
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
            Process_RpcC2G_ItemFindBackInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ItemFindBackInfo.OnTimeout(oArg);
        }
    }
}
