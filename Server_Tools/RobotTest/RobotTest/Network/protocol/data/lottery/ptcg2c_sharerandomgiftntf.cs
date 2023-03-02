using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/31 20:11:55

namespace XMainClient
{
    using DataType = KKSG.ShareRandomGiftData;

	class PtcG2C_ShareRandomGiftNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ShareRandomGiftNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 18823;
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
			Process_PtcG2C_ShareRandomGiftNtf.Process(this);
		}

	}
}
