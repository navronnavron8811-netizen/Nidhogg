#pragma once
#include "pch.h"

class RundownProtection {
private:
	EX_RUNDOWN_REF rundown;
	UCHAR isCompleted;

	_IRQL_requires_max_(DISPATCH_LEVEL)
		bool IsCompleted() {
		return InterlockedExchange8(reinterpret_cast<volatile CHAR*>(&isCompleted), TRUE) == FALSE;
	}

public:

	_IRQL_requires_max_(APC_LEVEL)
	RundownProtection() noexcept {
		rundown = { 0 };
		isCompleted = FALSE;
		ExInitializeRundownProtection(&rundown);
	}

	_IRQL_requires_max_(APC_LEVEL)
	~RundownProtection() {
		Complete();
	}

	_IRQL_requires_max_(APC_LEVEL)
	void Complete() {
		if (IsCompleted()) {
			ExWaitForRundownProtectionRelease(&rundown);
			InterlockedExchange8(reinterpret_cast<volatile CHAR*>(&isCompleted), TRUE);
			isCompleted = TRUE;
		}
	}

	_IRQL_requires_max_(APC_LEVEL)
	void Reinit() {
		ExReInitializeRundownProtection(&rundown);
	}

	_IRQL_requires_max_(DISPATCH_LEVEL)
	void Release() {
		ExReleaseRundownProtection(&rundown);
	}

	_IRQL_requires_max_(DISPATCH_LEVEL)
	bool Acquire() {
		if (IsCompleted())
			return false;
		return ExAcquireRundownProtection(&rundown) != FALSE;
	}
};

