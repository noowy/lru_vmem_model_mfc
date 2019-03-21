#include "stdafx.h"
#include "MainMemory.hpp"

using namespace std;

MainMemory::MainMemory(const int& max_size)
{
	head = new Page;
	head->next_item = nullptr;
	current_size = 0;
	this->max_size = max_size;
}

void MainMemory::insert_page(Page * new_item)
{
	if (!head->next_item)
	{
		head->next_item = new_item;
		current_size++;
		return;
	}
	
	new_item->next_item = head->next_item;
	head->next_item = new_item;

	current_size++;
}

void MainMemory::insert_page(int pageIdx)
{
	Page *new_entry = new Page;
	new_entry->pageIdx = pageIdx;
	new_entry->counter_buffer = page_call_counter;
	if (!head->next_item)
	{
		head->next_item = new_entry;
		current_size++;
		return;
	}

	new_entry->next_item = head->next_item;
	head->next_item = new_entry;

	current_size++;
}

bool MainMemory::remove_page(int pageIdx)
{
	if (!head->next_item)
		return false;

	Page *current_entry = head;
	Page *next_entry = head;

	while (next_entry)
	{
		if (next_entry->pageIdx == pageIdx)
		{
			current_entry->next_item = next_entry->next_item;
			current_size--;
			return true;
		}
		current_entry = next_entry;
		next_entry = current_entry->next_item;
	}

	return false;
}

bool MainMemory::remove_last_page()
{
	if (!head->next_item)
		return false;

	Page *current_entry = head;
	Page *next_entry = head;

	while (next_entry)
	{
		if (!next_entry->next_item)
		{
			current_entry->next_item = nullptr;
			delete next_entry;
			return true;
		}
		current_entry = next_entry;
		next_entry = current_entry->next_item;
	}

	return false;
}


bool MainMemory::remove_least_used_page()
{
	Page *current_entry = head;
	Page *next_entry = head->next_item;
	Page *least_used = get_least_used_page();

	while (next_entry)
	{
		if (current_entry->next_item == least_used)
		{
			current_entry->next_item = next_entry->next_item;
			delete least_used;
			return true;
		}
		current_entry = next_entry;
		next_entry = current_entry->next_item;
	}

	return true;
}

bool MainMemory::contains(int pageIdx)
{
	Page *current_entry = head;
	Page *next_entry = head;

	while (next_entry)
	{

		current_entry = next_entry;
		if (current_entry->pageIdx == pageIdx)
		{
			current_entry->counter_buffer = page_call_counter;
			return true;
		}
		next_entry = current_entry->next_item;
	}

	return false;
}

Page * MainMemory::get_least_used_page()
{
	Page *current_entry = head;
	Page *next_entry = head->next_item;
	Page *least_used = head->next_item;

	while (next_entry)
	{
		current_entry = next_entry;
		if (least_used->counter_buffer > current_entry->counter_buffer)
			least_used = current_entry;
		next_entry = current_entry->next_item;
	}

	return least_used;
}

int MainMemory::call_page(int pageIdx)
{
	if (contains(pageIdx))
	{
		return PAGE_CALLED;
	}
	else if (current_size < max_size)
	{
		insert_page(pageIdx);
		return PAGE_ADDED;
	}

	int least_called_pageIdx = get_least_used_page()->pageIdx;
	
	if (!remove_least_used_page())
		return MEMORY_ACCESS_ERROR;
	
	return least_called_pageIdx;

}

void MainMemory::set_max_size(const int & max_size)
{
	this->max_size = max_size;
}

int MainMemory::get_current_size()
{
	return current_size;
}
