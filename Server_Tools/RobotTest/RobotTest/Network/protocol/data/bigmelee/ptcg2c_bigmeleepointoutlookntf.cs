using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/10 15:40:02

namespace XMainClient
{
    using DataType = KKSG.BigMeleePointOutLook;

	class PtcG2C_BigMeleePointOutLookNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BigMeleePointOutLookNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 25027;
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
			Process_PtcG2C_BigMeleePointOutLookNtf.Process(this);
		}

	}
}
