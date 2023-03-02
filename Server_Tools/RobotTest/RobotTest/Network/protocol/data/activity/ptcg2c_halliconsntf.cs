using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/24 17:54:07

namespace XMainClient
{
    using DataType = KKSG.HallIconPara;

	class PtcG2C_HallIconSNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HallIconSNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 17871;
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
			Process_PtcG2C_HallIconSNtf.Process(this);
		}

	}
}
