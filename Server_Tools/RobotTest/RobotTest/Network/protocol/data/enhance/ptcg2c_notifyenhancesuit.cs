using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/7 22:28:32

namespace XMainClient
{
    using DataType = KKSG.NotifyEnhanceSuit;

	class PtcG2C_NotifyEnhanceSuit : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyEnhanceSuit()
		{

		}

		public override uint GetProtoType()
		{
			return 44091;
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
			Process_PtcG2C_NotifyEnhanceSuit.Process(this);
		}

	}
}
