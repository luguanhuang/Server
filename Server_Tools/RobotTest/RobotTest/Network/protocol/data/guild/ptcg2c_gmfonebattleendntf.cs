using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/18 10:28:23

namespace XMainClient
{
    using DataType = KKSG.GmfOneBattleEnd;

	class PtcG2C_GmfOneBattleEndNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfOneBattleEndNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 61740;
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
			Process_PtcG2C_GmfOneBattleEndNtf.Process(this);
		}

	}
}
