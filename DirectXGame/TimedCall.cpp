#include "TimedCall.h"

void TimedCall::Update() {
	if (isFinished_) {
		return;
	}
	timer_--;
	if (timer_ <= 0) {
		callback_();
		isFinished_ = true;
	}
}

