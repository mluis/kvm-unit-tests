#ifndef _ASMARM64_MMU_H_
#define _ASMARM64_MMU_H_
/*
 * Copyright (C) 2014, Red Hat Inc, Andrew Jones <drjones@redhat.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.
 */
#include <asm/barrier.h>
#include <asm/processor.h>

#define PMD_SECT_UNCACHED	PMD_ATTRINDX(MT_DEVICE_nGnRE)
#define PTE_UNCACHED		PTE_ATTRINDX(MT_DEVICE_nGnRE)
#define PTE_WBWA		PTE_ATTRINDX(MT_NORMAL)

static inline void flush_tlb_all(void)
{
	dsb(ishst);
        if (current_level() == CurrentEL_EL2 && !cpu_el2_e2h_is_set())
                asm("tlbi       alle2is");
        else
                asm("tlbi       vmalle1is");
	dsb(ish);
	isb();
}

static inline void flush_tlb_page(unsigned long vaddr)
{
	unsigned long page = vaddr >> 12;
	dsb(ishst);
        if (current_level() == CurrentEL_EL2 && !cpu_el2_e2h_is_set())
                asm("tlbi       vae2is, %0" :: "r" (page));
        else
                asm("tlbi       vaae1is, %0" :: "r" (page));
	dsb(ish);
	isb();
}

static inline void flush_dcache_addr(unsigned long vaddr)
{
	asm volatile("dc civac, %0" :: "r" (vaddr));
}

#include <asm/mmu-api.h>

#endif /* _ASMARM64_MMU_H_ */
