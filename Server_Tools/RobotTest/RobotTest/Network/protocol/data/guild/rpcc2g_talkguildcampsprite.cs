using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/26 11:13:22

namespace XMainClient
{
	using ArgDataType = KKSG.TalkGuildCampSpriteArg;
    using ResDataType = KKSG.TalkGuildCampSpriteRes;

    class RpcC2G_TalkGuildCampSprite : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_TalkGuildCampSprite()
        {
        }

        public override uint GetRpcType()
        {
            return 5683;
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
            Process_RpcC2G_TalkGuildCampSprite.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TalkGuildCampSprite.OnTimeout(oArg);
        }
    }
}
