using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/1/18 17:25:44

namespace XMainClient
{
    using DataType = KKSG.ChangeSupplementNtf;

	class PtcG2C_ChangeSupplementNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ChangeSupplementNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 11250;
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
			Process_PtcG2C_ChangeSupplementNtf.Process(this);
		}

	}
}
