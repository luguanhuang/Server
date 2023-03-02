using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/16 10:07:52

namespace XMainClient
{
	using ArgDataType = KKSG.QuerySceneTimeArg;
    using ResDataType = KKSG.QuerySceneTimeRes;

    class RpcC2G_QuerySceneTime : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QuerySceneTime()
        {
        }

        public override uint GetRpcType()
        {
            return 39595;
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
            Process_RpcC2G_QuerySceneTime.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QuerySceneTime.OnTimeout(oArg);
        }
    }
}
