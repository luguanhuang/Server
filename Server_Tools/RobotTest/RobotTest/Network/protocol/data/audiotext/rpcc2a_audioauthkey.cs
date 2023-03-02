using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/21 10:08:34

namespace XMainClient
{
	using ArgDataType = KKSG.AudioAuthKeyArg;
    using ResDataType = KKSG.AudioAuthKeyRes;

    class RpcC2A_AudioAuthKey : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2A_AudioAuthKey()
        {
        }

        public override uint GetRpcType()
        {
            return 19391;
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
            Process_RpcC2A_AudioAuthKey.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2A_AudioAuthKey.OnTimeout(oArg);
        }
    }
}
