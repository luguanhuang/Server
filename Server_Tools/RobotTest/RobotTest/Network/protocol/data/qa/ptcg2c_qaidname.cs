using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/26 16:10:04

namespace XMainClient
{
    using DataType = KKSG.QAIdName;

	class PtcG2C_QAIDName : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_QAIDName()
		{

		}

		public override uint GetProtoType()
		{
			return 18100;
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
			Process_PtcG2C_QAIDName.Process(this);
		}

	}
}
