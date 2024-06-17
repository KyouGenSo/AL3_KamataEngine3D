#pragma once
#include <functional>

class TimedCall {
public:
	TimedCall(std::function<void()> callback, uint32_t time) : callback_(callback), timer_(time), isFinished_(false) {}

	void Update();
	bool IsFinished() { return isFinished_; }

private:
	std::function<void()> callback_;
	uint32_t timer_;
	bool isFinished_;
};