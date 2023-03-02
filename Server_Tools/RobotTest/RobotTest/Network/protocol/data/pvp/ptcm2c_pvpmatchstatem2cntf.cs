using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/19 19:45:13

namespace XMainClient
{
    using DataType = KKSG.PvpMatchState;

	class PtcM2C_PvpMatchStateM2CNTF : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_PvpMatchStateM2CNTF()
		{

		}

		public override uint GetProtoType()
		{
			return 15248;
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
			Process_PtcM2C_PvpMatchStateM2CNTF.Process(this);
		}

	}
}
