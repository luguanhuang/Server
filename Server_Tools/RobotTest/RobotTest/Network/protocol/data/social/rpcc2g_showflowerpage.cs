using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/23 16:11:28

namespace XMainClient
{
	using ArgDataType = KKSG.ShowFlowerPageArg;
    using ResDataType = KKSG.ShowFlowerPageRes;

    class RpcC2G_ShowFlowerPage : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ShowFlowerPage()
        {
        }

        public override uint GetRpcType()
        {
            return 47831;
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
            Process_RpcC2G_ShowFlowerPage.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ShowFlowerPage.OnTimeout(oArg);
        }
    }
}
