#pragma once

#pragma pack(push, 1)

namespace GameCore
{
	struct PACKET_HEADER
	{
		int16 size;
		int16 type;
	};

	static const int32 PACKET_HEADER_LENGTH = sizeof(PACKET_HEADER);
	static const int32 PACKET_SIZE_LENGTH = sizeof(int16);
	static const int32 PACKET_TYPE_LENGTH = sizeof(int16);
}

#pragma pack(pop)
