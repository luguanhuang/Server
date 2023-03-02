using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/10 19:28:33

namespace XMainClient
{
    using DataType = KKSG.InvUnfState;

	class PtcG2C_InvUnfStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_InvUnfStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 45628;
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
			Process_PtcG2C_InvUnfStateNtf.Process(this);
		}

	}
}
