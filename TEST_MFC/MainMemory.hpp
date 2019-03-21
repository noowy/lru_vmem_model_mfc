#ifndef MAINMEMORY_HPP
#define MAINMEMORY_HPP

extern int page_call_counter;

enum 
{ 
	MEMORY_ACCESS_ERROR = -3, 
	PAGE_ADDED = -2, 
	PAGE_CALLED = -1, 
	EMPTY_QUEUE = 0 
};

struct Page
{
	int pageIdx;
	int counter_buffer;
	Page *next_item = nullptr;
};

class MainMemory
{
	Page *head;
	int current_size;
	int max_size;

public:
	MainMemory(const int& max_size);
	
	void insert_page(Page *new_item);
	
	void insert_page(int page);
	
	bool remove_page(int page);
	
	bool remove_last_page();
	
	bool remove_least_used_page();
	
	bool contains(int page);
	
	Page* get_least_used_page();

	/* 
		returns PAGE_ADDED if the page is not present in the pool and there is space to add it
	 	returns PAGE_CALLED if the page is present in the pool
	 	else returns index of the page that was replaced (or MEMORY_ACCESS_ERROR if it couldn't)
	*/
	int call_page(int pageIdx);
	
	void set_max_size(const int& max_size);

	int get_current_size();
};

#endif