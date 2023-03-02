using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/3 11:47:08

namespace XMainClient
{
    using DataType = KKSG.DisplayAddItemArg;

	class PtcG2C_DisplayAddItem : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_DisplayAddItem()
		{

		}

		public override uint GetProtoType()
		{
			return 55159;
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
			Process_PtcG2C_DisplayAddItem.Process(this);
		}

	}
}
