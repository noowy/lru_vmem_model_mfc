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
		break;
	}
}

void MemoryController::onProcessLaunched()
{
	
}

void MemoryController::onProcessPaused()
{
}
