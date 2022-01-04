#pragma once

class Session
{
public:
	virtual void OnConnect() = 0;
	virtual void OnDisconnect() = 0;
	
private:
	std::atomic<int> mState;
};

