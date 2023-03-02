using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/15 16:58:48

namespace XMainClient
{
	using ArgDataType = KKSG.RefreshFashionArg;
    using ResDataType = KKSG.RefreshFashionRes;

    class RpcC2G_RefreshFashionChallenge : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_RefreshFashionChallenge()
        {
        }

        public override uint GetRpcType()
        {
            return 38637;
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
            Process_RpcC2G_RefreshFashionChallenge.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_RefreshFashionChallenge.OnTimeout(oArg);
        }
    }
}
