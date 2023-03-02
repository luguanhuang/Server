using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/25 16:04:51

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcM2C_HeroBattleStartMatch : Protocol
	{
		//public DataType Data = new DataType();

		public PtcM2C_HeroBattleStartMatch()
		{

		}

		public override uint GetProtoType()
		{
			return 43339;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcM2C_HeroBattleStartMatch.Process(this);
		}

	}
}
