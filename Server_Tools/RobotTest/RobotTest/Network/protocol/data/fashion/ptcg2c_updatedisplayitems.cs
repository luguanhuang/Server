using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/4 11:44:22

namespace XMainClient
{
    using DataType = KKSG.UpdateDisplayItems;

	class PtcG2C_UpdateDisplayItems : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UpdateDisplayItems()
		{

		}

		public override uint GetProtoType()
		{
			return 12217;
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
			Process_PtcG2C_UpdateDisplayItems.Process(this);
		}

	}
}
