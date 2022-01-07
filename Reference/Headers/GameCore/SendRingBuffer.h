#pragma once

#include "CoreCommon.h"
#include "FastSpinLock.h"


namespace GameCore
{
	class SendRingBuffer
	{
	public:
		SendRingBuffer() = default;

		~SendRingBuffer()
		{
			delete[] m_pRingBuffer;
		}


		bool Create(const int ringBufferSize)
		{
			if (ringBufferSize == INVALID_VALUE)
			{
				return false;
			}

			m_RingBufferSize = ringBufferSize;

			delete[] m_pRingBuffer;
			m_pRingBuffer = new char[m_RingBufferSize];

			Init();
			return true;
		}

		void Init()
		{
			FastSpinlockGuard Lock(mSpinLock);

			m_ReadPos = 0;
			m_WritePos = 0;
		}

		bool AddSendData(const int dataSize, const char* pData)
		{
			FastSpinlockGuard Lock(mSpinLock);

			if (auto size = m_RingBufferSize - m_WritePos; size < dataSize)
			{
				auto remain = m_WritePos - m_ReadPos;
				if (remain > 0)
				{
					CopyMemory(&m_pRingBuffer[0], &m_pRingBuffer[m_ReadPos], remain);
				}

				m_WritePos = remain;
				m_ReadPos = 0;
			}

			if (auto size = m_RingBufferSize - m_WritePos; size < dataSize)
			{
				return false;
			}

			CopyMemory(&m_pRingBuffer[m_WritePos], pData, dataSize);
			return true;
		}

		std::tuple<int, char*> GetSendAbleData(const int maxSize)
		{
			FastSpinlockGuard Lock(mSpinLock);

			auto remain = m_WritePos - m_ReadPos;
			if (remain <= 0)
			{
				return { 0, nullptr };
			}

			if (remain > maxSize)
			{
				remain = maxSize;
			}

			return { remain, &m_pRingBuffer[m_ReadPos] };

		}

		void ForwardReadPos(const int forwardLength)
		{
			FastSpinlockGuard Lock(mSpinLock);
			m_ReadPos += forwardLength;
		}


	private:
		FastSpinlock mSpinLock;

		char* m_pRingBuffer = nullptr;
		int m_RingBufferSize = INVALID_VALUE;
		int m_ReadPos = 0;
		int m_WritePos = 0;
	};
}