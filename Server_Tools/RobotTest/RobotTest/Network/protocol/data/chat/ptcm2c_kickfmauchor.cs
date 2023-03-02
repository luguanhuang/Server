using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/16 14:29:26

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcM2C_KickFMAuchor : Protocol
	{
		//public DataType Data = new DataType();

		public PtcM2C_KickFMAuchor()
		{

		}

		public override uint GetProtoType()
		{
			return 33806;
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
			Process_PtcM2C_KickFMAuchor.Process(this);
		}

	}
}
