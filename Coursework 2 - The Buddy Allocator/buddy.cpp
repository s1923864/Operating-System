/*
 * The Buddy Page Allocator
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 2
 */

#include <infos/mm/page-allocator.h>
#include <infos/mm/mm.h>
#include <infos/kernel/kernel.h>
#include <infos/kernel/log.h>
#include <infos/util/math.h>
#include <infos/util/printf.h>

using namespace infos::kernel;
using namespace infos::mm;
using namespace infos::util;

#define MAX_ORDER	18

/**
 * A buddy page allocation algorithm.
 */
class BuddyPageAllocator : public PageAllocatorAlgorithm
{
private:
	/** Given a page descriptor, and an order, returns the buddy PGD.  The buddy could either be
	 * to the left or the right of PGD, in the given order.
	 * @param pgd The page descriptor to find the buddy for.
	 * @param order The order in which the page descriptor lives.
	 * @return Returns the buddy of the given page descriptor, in the given order.
	 */
	PageDescriptor *buddy_of(PageDescriptor *pgd, int order)
	{
        // TODO: Implement me!
        pfn_t position = sys.mm().pgalloc().pgd_to_pfn(pgd);
        int size = 1 << order;
        position = position/size;
        if((position % 2) == 1){
        return pgd - size;
        }
        else {
        return pgd + size;
        }
        
        
        
	}

	/**
	 * Given a pointer to a block of free memory in the order "source_order", this function will
	 * split the block in half, and insert right half into the order below and keep left half. If there is no block in current order, this    function will look up for higher order until it finds availible block and then keep splitting block down until it meets the order requirement
	 * @param block_pointer A pointer to a PageDescriptor containing the beginning of a block of free memory.
	 * @param source_order The order in which the block of free memory exists.
	 * @return Returns the left-hand-side of the new block.
	 */
	PageDescriptor *split_block(PageDescriptor *block_pointer, int source_order)
	{
        // TODO: Implement me!
        pfn_t position = sys.mm().pgalloc().pgd_to_pfn(block_pointer);
        PageDescriptor *lhs_block = block_pointer;
        int i = source_order;
        int num = source_order;
        bool find = false;
        for(int i = source_order;i < 19;i++){
        if(_free_areas[i] != NULL){
        find = true;
        num = i;
        break;
        }
        }
        if(find == false ){
        return NULL;
        }
        else{
        return split_some_blocks(num - source_order + 1, num);
        }
        
       
	}
	
	
	
	
	/**
	 * Given the split times in total and the page order that begin to split, this function will
	 *1. split the block in half
	 *2. insert left half and right half to the order below
	 *3. do step 1 for left half
	 * and this function will only stop splitting when it finish its split times required.
	 * This function is helper function for split_block.
	 * @param split_times times required to split.
	 * @param page_order_to_split The order in which the first block will begin to split. 
	 * @return Returns the final left-hand-side of the new block.
	 */
	PageDescriptor * split_some_blocks(int split_times, int page_order_to_split){
	
	PageDescriptor *page = _free_areas[page_order_to_split];
	int order = page_order_to_split;
	break_from_source(page, order);
	while(split_times > 0){

        add_to_destination(page + (1 << (order - 1)), order - 1);
	order--;
	split_times--;
	}
	return page;
	}
	
	
	
	
	
	

	/**
	 * Takes a block in the given source order, and merges it (and its buddy) into the next order and if there is no buddy in current order, this function won't merge blocks. If the result have buddy at next order, it will keep merging until there is no buddy in current order. 
	 * @param block_pointer A pointer to a PageDescriptor.
	 * @param source_order The order in which the pair of blocks live.
	 * @return Returns the new slot that points to the merged block.
	 */
	PageDescriptor *merge_block(PageDescriptor *block_pointer, int source_order)
	{
        // TODO: Implement me!
        
        bool find = false;
        PageDescriptor *merge = block_pointer;
        int order = source_order;
        PageDescriptor *b = _free_areas[order];
        PageDescriptor *buddy = buddy_of(block_pointer,order);
        while(order < 18){
        b = _free_areas[order];
        buddy = buddy_of(merge,order);
        find = false;
        while(b != NULL){
        if(b == buddy){
        merge = merge_one_block(merge, order);
        find = true;
        break;
        }
        b = b -> next_free;
        }
        if(find == false){
        break;
        }
        
        order++;
        
        }
        
        return merge;
        
	}
	
	
	
	/**
	 * Takes a block in the given source order, and merges it (and its buddy) into the next order. This function is helper function for merge_block function.
	 * @param block_pointer A pointer to a PageDescriptor containing a block in the pair to merge.
	 * @param source_order The order in which the pair of blocks live.
	 * @return Returns the new slot that points to the merged block.
	 */
	PageDescriptor *merge_one_block(PageDescriptor *block_pointer, int source_order){
	
	pfn_t position = sys.mm().pgalloc().pgd_to_pfn(block_pointer);
        pfn_t size = 1 << source_order;
        position = position/size;
        PageDescriptor *buddy = buddy_of(block_pointer,source_order);
        
        if((position % 2) == 1){
        change_block_position(buddy,source_order,source_order+1);
        break_from_source(block_pointer,source_order);
        return buddy;
        }   
        else{
        change_block_position(block_pointer,source_order,source_order+1);
        break_from_source(buddy,source_order);
        return block_pointer;
        }
	
	}
	
	


        /**
	 * This function will remove a block from source order of _free_areas.
	 * @param block_pointer A pointer to a PageDescriptor that need to be removed.
	 * @param source_order The order in which the block live.
	 */
        void break_from_source(PageDescriptor *block_pointer, int source_order){
        
        if(block_pointer -> prev_free == NULL && block_pointer -> next_free == NULL){
        _free_areas[source_order] = NULL;
        }
        else if (block_pointer -> prev_free == NULL){
	_free_areas[source_order] = block_pointer -> next_free;
	if(block_pointer -> next_free != NULL){
	(block_pointer -> next_free) -> prev_free = NULL;
	}
	}
	else if(block_pointer -> next_free == NULL){
	(block_pointer -> prev_free) -> next_free = NULL;
	}
	else{
	(block_pointer -> prev_free) -> next_free = block_pointer -> next_free;
	(block_pointer -> next_free) -> prev_free = block_pointer -> prev_free;
	}
	
	block_pointer -> prev_free = NULL;
	block_pointer -> next_free = NULL;
        }
        
        
        
        /**
	 * This function will add a block to destination order of _free_areas.
	 * @param block_pointer A pointer to a PageDescriptor that need to be added.
	 * @param destination_order The order in which the block wants to be added.
	 */
        void add_to_destination(PageDescriptor *block_pointer, int destination_order){
        
        if(_free_areas[destination_order] != NULL ){
        PageDescriptor *reference_1 = _free_areas[destination_order];
        PageDescriptor *reference_2 = _free_areas[destination_order] -> next_free;
        if(sys.mm().pgalloc().pgd_to_pfn(reference_1) < sys.mm().pgalloc().pgd_to_pfn(block_pointer)){
        while(reference_2 != NULL){
        
        if(sys.mm().pgalloc().pgd_to_pfn(reference_1) < sys.mm().pgalloc().pgd_to_pfn(block_pointer) && sys.mm().pgalloc().pgd_to_pfn(block_pointer) < sys.mm().pgalloc().pgd_to_pfn(reference_2)){
        break;
        }
        reference_1 = reference_1 -> next_free;
        reference_2 = reference_2 -> next_free;
        }
        if(reference_2 == NULL){
        reference_1 -> next_free = block_pointer;
        block_pointer -> prev_free = reference_1;
        }
        else{
        reference_1 -> next_free = block_pointer;
        block_pointer -> prev_free = reference_1;
        reference_2 -> prev_free = block_pointer;
        block_pointer -> next_free = reference_2;
        }
        }
        else{
        block_pointer -> next_free = reference_1;
        reference_1 -> prev_free = block_pointer;
        _free_areas[destination_order] = block_pointer;
        }
        }
        else{
        _free_areas[destination_order] = block_pointer;
        }
        }
        
        
        
        
        
        
        
        
        
        /**
	 * This function will change the order of a block from source order to destination order of _free_areas.
	 * @param block_pointer A pointer to a PageDescriptor that want to change position.
	 * @param source_order The order in which the block wants to be removed.
	 * @param destination_order The order in which the block wants to be added.
	 */
        void change_block_position(PageDescriptor *block_pointer, int source_order, int destination_order){
	
	break_from_source(block_pointer, source_order);
	
	add_to_destination(block_pointer, destination_order);
	}





        /**
	 * This function will return the order that is suitable for aligned(or not aligned) continuous pages to insert.
	 * @param begin The pfn_t address where continuous page descriptors begin.
	 * @param count The amount of page descriptors that want to insert. 
	 */
        int insertion_order_decision(pfn_t begin, uint64_t count){
         
         bool zero = begin == 0;
         int i = 18;
         while(i >= 0){
         if(((begin % (1 << i)) == 0 || zero) && count >= (1 << i)){
         return i;
         }
         i--;
         }
         
         return 0;
         
        }

        
        
        
        /**
	 * This function will returns begining page descriptor which need to be removed in a given block and this function is helper function for find_some_pages_and_remove_some_pages.
	 * @param target_to_remove the begining page descriptor of a target that want to be removed.
	 * @param count The amount of page descriptors for a target.
	 * @param search begining page descriptor of a page block in _free_areas.
	 * @param order The order in which the param search is in. 
	 * @return Returns begining page descriptor which need to be removed in a given block.
	 */
        PageDescriptor *find_start_page(PageDescriptor *target_to_remove, uint64_t count, PageDescriptor *search, int order){
        
        pfn_t target_begin = sys.mm().pgalloc().pgd_to_pfn(target_to_remove);
        pfn_t target_end = sys.mm().pgalloc().pgd_to_pfn(target_to_remove + count - 1);
        pfn_t search_begin = sys.mm().pgalloc().pgd_to_pfn(search);
        pfn_t search_end = sys.mm().pgalloc().pgd_to_pfn(search + (1 << order) - 1 );
        
        if((target_end < search_begin) || (target_begin > search_end)){
        
        return NULL;
        
        }
        else if(target_begin >= search_begin){
        
        return target_to_remove;
        
        }
        else{
        
        return sys.mm().pgalloc().pfn_to_pgd(search_begin);
        
        }
        
        }
        
        


        /**
	 * This function will remove the maximum fragment of a continuous page block contained in a given page block in _free_areas.
	 * @param target_to_remove the begining page descriptor of a target that want to be removed.
	 * @param count The amount of page descriptors for a target.
	 * @param search begining page descriptor of a block in _free_areas.
	 * @param order The order in which the param search is in. 
	 * 
	 */
        bool find_some_pages_and_remove_some_pages(PageDescriptor *target_to_remove, uint64_t count, PageDescriptor *search, int order){
        PageDescriptor *start_page = find_start_page(target_to_remove, count, search, order);
        PageDescriptor *part_1 = NULL;
        int part_1_num = 0;
        PageDescriptor *part_2 = NULL;
        int part_2_num = 0;
        
    
        if(start_page != NULL){
        pfn_t begin = sys.mm().pgalloc().pgd_to_pfn(start_page);
        pfn_t search_begin = sys.mm().pgalloc().pgd_to_pfn(search);
        pfn_t target_end = sys.mm().pgalloc().pgd_to_pfn(target_to_remove + count - 1);
        pfn_t search_end = sys.mm().pgalloc().pgd_to_pfn(search + (1 << order) - 1);
        if(search_begin < begin ){
        part_1 = search;
        part_1_num = begin - search_begin;
        }
        if(target_end < search_end ){
        part_2 = target_to_remove + count;
        part_2_num = search_end - target_end;
        }
        
        break_from_source(search, order);
        if(part_1 != NULL){ 
        
        insert_page_range(part_1, part_1_num);
       
        }
        if(part_2 != NULL){
     
        insert_page_range(part_2, part_2_num);
        
        }
        return true;
     
        }
        else{
        
        return false;
        } 
        
        }


public:
	/**
	 * Allocates 2^order number of contiguous pages
	 * @param order The power of two, of the number of contiguous pages to allocate.
	 * @return Returns a pointer to the first page descriptor for the newly allocated page range, or NULL if
	 * allocation failed.
	 */
	PageDescriptor *allocate_pages(int order) override
	{
        // TODO: Implement me!
       
        if(_free_areas[order] == NULL){
        if(order < 18){
        return split_block(_free_areas[order + 1], order + 1);
        }
        else{
        return NULL;
        }
        }
        else{
        PageDescriptor *allocated_pages = _free_areas[order];
        break_from_source(_free_areas[order], order);
        return allocated_pages;
        }
        
	}

    /**
	 * Frees 2^order contiguous pages.
	 * @param pgd A pointer to an array of page descriptors to be freed.
	 * @param order The power of two number of contiguous pages to free.
	 */
    void free_pages(PageDescriptor *pgd, int order) override
    {
        // TODO: Implement me!
       
        insert_page_range(pgd, (1 << order));
     
        
    }

    /**
     * Marks a range of pages as available for allocation.
     * @param start A pointer to the first page descriptors to be made available.
     * @param count The number of page descriptors to make available.
     */
    virtual void insert_page_range(PageDescriptor *start, uint64_t count) override
    {
        // TODO: Implement me!
       
        
        PageDescriptor *insertion_block = start;
        pfn_t begin = sys.mm().pgalloc().pgd_to_pfn(start);
        int insertion_number = 0;
        int insertion_order = 0;
        while(count > 0){
        insertion_order = insertion_order_decision(begin, count);
        insertion_number = 1 << insertion_order;
        add_to_destination(insertion_block, insertion_order);
        merge_block(insertion_block, insertion_order);
        insertion_block = insertion_block + insertion_number;
        begin = begin + insertion_number;
        count = count - insertion_number;
        }
        
        
    }

    /**
     * Marks a range of pages as unavailable for allocation.
     * @param start A pointer to the first page descriptors to be made unavailable.
     * @param count The number of page descriptors to make unavailable.
     */
    virtual void remove_page_range(PageDescriptor *start, uint64_t count) override
    {
        // TODO: Implement me!
        PageDescriptor *search = NULL;
        int order_num = 18;
        bool find = false;
        while(order_num >= 0){
        search = _free_areas[order_num];
        while(search != NULL){
        if(find_some_pages_and_remove_some_pages(start, count, search, order_num)){
        find = true;
        }
        search = search -> next_free;
        }
        order_num--;
        
        }
        assert(find); // ensure the target to remove is in free list
        
        
    }

	/**
	 * Initialises the allocation algorithm.
	 * @return Returns TRUE if the algorithm was successfully initialised, FALSE otherwise.
	 */
	bool init(PageDescriptor *page_descriptors, uint64_t nr_page_descriptors) override
	{
        // TODO: Implement me!
       
        return true;
        
	}

	/**
	 * Returns the friendly name of the allocation algorithm, for debugging and selection purposes.
	 */
	const char* name() const override { return "buddy"; }

	/**
	 * Dumps out the current state of the buddy system
	 */
	void dump_state() const override
	{
		// Print out a header, so we can find the output in the logs.
		mm_log.messagef(LogLevel::DEBUG, "BUDDY STATE:");

		// Iterate over each free area.
		for (unsigned int i = 0; i < ARRAY_SIZE(_free_areas); i++) {
			char buffer[256];
			snprintf(buffer, sizeof(buffer), "[%d] ", i);

			// Iterate over each block in the free area.
			PageDescriptor *pg = _free_areas[i];
			while (pg) {
				// Append the PFN of the free block to the output buffer.
				snprintf(buffer, sizeof(buffer), "%s%lx ", buffer, sys.mm().pgalloc().pgd_to_pfn(pg));
				pg = pg->next_free;
			}

			mm_log.messagef(LogLevel::DEBUG, "%s", buffer);
		}
	}


private:
	PageDescriptor *_free_areas[MAX_ORDER+1];
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

/*
 * Allocation algorithm registration framework
 */
RegisterPageAllocator(BuddyPageAllocator);
