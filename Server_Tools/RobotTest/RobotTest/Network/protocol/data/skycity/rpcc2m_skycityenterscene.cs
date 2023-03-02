using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/1 10:40:23

namespace XMainClient
{
	using ArgDataType = KKSG.SkyCityEnterArg;
    using ResDataType = KKSG.SkyCityEnterRes;

    class RpcC2M_SkyCityEnterScene : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_SkyCityEnterScene()
        {
        }

        public override uint GetRpcType()
        {
            return 14959;
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
            Process_RpcC2M_SkyCityEnterScene.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_SkyCityEnterScene.OnTimeout(oArg);
        }
    }
}
