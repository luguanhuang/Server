using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/7 20:28:35

namespace XMainClient
{
    using DataType = KKSG.IBShopIcon;

	class PtcG2C_IBShopIcon : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_IBShopIcon()
		{

		}

		public override uint GetProtoType()
		{
			return 56800;
		}

		public override void Serialize(MemoryStream stream)
		{
			Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcG2C_IBShopIcon.Process(this);
		}

	}
}
