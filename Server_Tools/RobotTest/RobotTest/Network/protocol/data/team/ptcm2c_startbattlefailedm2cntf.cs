using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/7 17:07:48

namespace XMainClient
{
    using DataType = KKSG.StartBattleFailedRes;

	class PtcM2C_StartBattleFailedM2CNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_StartBattleFailedM2CNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 20444;
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
			Process_PtcM2C_StartBattleFailedM2CNtf.Process(this);
		}

	}
}
