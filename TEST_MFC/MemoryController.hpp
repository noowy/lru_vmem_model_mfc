#pragma once
#ifndef MEMORY_CONTROLLER_HPP
#define MEMORY_CONTROLLER_HPP
#include "MainMemory.hpp"

class CTESTMFCDlg;

class MainMemory;

extern int page_call_counter;

class MemoryController
{
private:
	CTESTMFCDlg * view;
	MainMemory *mainMemory;

public:
	MemoryController(CTESTMFCDlg *view);

	void onPageCalled();
	void onProcessLaunched();
	void onProcessPaused();
};

#endif