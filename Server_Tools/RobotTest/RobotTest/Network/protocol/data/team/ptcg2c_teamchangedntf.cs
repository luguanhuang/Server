using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/15 17:50:10

namespace XMainClient
{
    using DataType = KKSG.TeamChanged;

	class PtcG2C_TeamChangedNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TeamChangedNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 34165;
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
			Process_PtcG2C_TeamChangedNtf.Process(this);
		}

	}
}
