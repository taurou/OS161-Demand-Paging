#include <vm_tlb.h>
#include <mips/tlb.h>
#include <spl.h>
#include <vm.h>
#include <lib.h>

int victim = 0;

void tlb_invalidate(void)
{
    int spl, i;

    /* Disable interrupts on this CPU while frobbing the TLB. */
    spl = splhigh();

    for (i = 0; i < NUM_TLB; i++)
    {
        tlb_write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
    }

    splx(spl);
}

void tlb_insert(vaddr_t vaddr, paddr_t paddr, bool ro)
{
    int spl;
    uint32_t ehi, elo;

    /* Make sure it's page-aligned */
    KASSERT((paddr & PAGE_FRAME) == paddr);

    /* Disable interrupts on this CPU while frobbing the TLB. */
    spl = splhigh();

    ehi = vaddr;
    elo = paddr | TLBLO_VALID;
    if (!ro)
    {
        elo = elo | TLBLO_DIRTY;
    }
    tlb_write(ehi, elo, victim);
    victim = (victim + 1) % NUM_TLB;

    splx(spl);
}

void tlb_remove(vaddr_t vaddr)
{
    int index;

    index = tlb_probe(vaddr, 0);
    if (index >= 0)
        tlb_write(TLBHI_INVALID(index), TLBLO_INVALID(), index);
}