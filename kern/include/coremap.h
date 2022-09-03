#ifndef _COREMAP_H_
#define _COREMAP_H_

#include <pt.h>
#include "opt-rudevm.h"

#if OPT_RUDEVM

struct coremap_entry
{
    unsigned int        cm_used : 1;
    unsigned int        cm_lock : 1;            /*  the frame is locked if it is swapping out    */
    unsigned int        cm_allocsize : 16;      //TODO should we change it? 
    struct pt_entry     *cm_ptentry;            /*  page table entry of the page living 
                                                    in this frame, NULL if kernel page  */
};

void        coremap_bootstrap(void);
paddr_t     coremap_getppages(int npages, struct pt_entry *ptentry);
void        coremap_freeppages(paddr_t addr);

#endif /* OPT_RUDEVM */

#endif /* _COREMAP_H_ */
