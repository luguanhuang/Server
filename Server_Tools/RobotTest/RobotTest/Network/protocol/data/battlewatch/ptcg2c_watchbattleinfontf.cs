using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/13 20:17:39

namespace XMainClient
{
    using DataType = KKSG.WatchBattleData;

	class PtcG2C_WatchBattleInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WatchBattleInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 23415;
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
			Process_PtcG2C_WatchBattleInfoNtf.Process(this);
		}

	}
}
