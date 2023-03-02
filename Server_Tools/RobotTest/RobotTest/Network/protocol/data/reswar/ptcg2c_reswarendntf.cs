using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/3 16:15:39

namespace XMainClient
{
    using DataType = KKSG.ResWarEnd;

	class PtcG2C_ResWarEndNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ResWarEndNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4850;
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
			Process_PtcG2C_ResWarEndNtf.Process(this);
		}

	}
}
