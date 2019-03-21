#pragma once
#include "stdafx.h"
#include "MemoryController.hpp"
#include "TEST_MFCDlg.h"

MemoryController::MemoryController(CTESTMFCDlg * view)
{
	this->view = view;
	mainMemory = new MainMemory(0);
}

void MemoryController::onPageCalled()
{
	tryPageCall();
}

void MemoryController::onProcessLaunched()
{
	if (isPaused)
		isPaused = false;
		cv.notify_one();
	std::thread processThread(&MemoryController::launchProcess, this);
	processThread.detach();
}

void MemoryController::onProcessPaused()
{
	isPaused = true;
}

void MemoryController::launchProcess()
{
	std::unique_lock<std::mutex> lock(mtx);
	isLaunched = true;
	while (view->getCurrentQueueSize() > 0)
	{
		if (isPaused)
		{
			isLaunched = false;
			cv.wait(lock);
			isLaunched = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		tryPageCall();
		page_call_counter++;
	}
	isLaunched = false;
}

void MemoryController::tryPageCall()
{
	mainMemory->set_max_size(view->getPagePoolSize());

	int calledPageIdx = view->popPageFromQueue();
	if (calledPageIdx == EMPTY_QUEUE)
		return;

	view->updatePageQueueView();

	int answer = mainMemory->call_page(calledPageIdx);

	switch (answer)
	{
	case PAGE_CALLED:
		view->updatePagePoolView();
		break;

	case PAGE_ADDED:
		view->addPageToPool(calledPageIdx);
		view->updatePagePoolView();
		break;

	default:
		view->replacePageInPool(answer, calledPageIdx);
		view->updatePagePoolView();
		view->incrementPageFaults();
		break;
	}
}
