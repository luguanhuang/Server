using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/11 16:34:17

namespace XMainClient
{
    using DataType = KKSG.LeagueBattleMatchTimeoutNtf;

	class PtcM2C_LeagueBattleMatchTimeoutNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LeagueBattleMatchTimeoutNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 31012;
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
			Process_PtcM2C_LeagueBattleMatchTimeoutNtf.Process(this);
		}

	}
}
