using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/24 12:30:20

namespace XMainClient
{
	using ArgDataType = KKSG.GardenExpelSpriteArg;
    using ResDataType = KKSG.GardenExpelSpriteRes;

    class RpcC2M_GardenExpelSprite : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GardenExpelSprite()
        {
        }

        public override uint GetRpcType()
        {
            return 3250;
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
            Process_RpcC2M_GardenExpelSprite.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GardenExpelSprite.OnTimeout(oArg);
        }
    }
}
