using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/29 17:19:12

namespace XMainClient
{
	using ArgDataType = KKSG.QuickPKArg;
    using ResDataType = KKSG.QuickPKRes;

    class RpcC2G_QuickPK : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QuickPK()
        {
        }

        public override uint GetRpcType()
        {
            return 40778;
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
            Process_RpcC2G_QuickPK.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QuickPK.OnTimeout(oArg);
        }
    }
}
