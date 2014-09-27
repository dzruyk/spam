// implement fork from user space

#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
void
pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	uint32_t pn;
	uint32_t perm;
	int r;

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at uvpt
	//   (see <inc/memlayout.h>).

	// LAB 4: Your code here.
	if ((err & FEC_WR) == 0)
		panic("not expectable error at %p (errcode %x)", addr, err);
	// Allign to pagesize

	pn = (uint32_t)addr / PGSIZE;
	///cprintf("[%x] pgfault eip = %p, fault addr = %p, err = %p, pn = %p\n", sys_getenvid(), utf->utf_eip, addr, err, pn);
	addr = (void *) PTE_ADDR(addr);

	perm = uvpt[pn] & 0xfff; 
	if (uvpd[pn >> 10] == 0 ||
	    (perm & PTE_P) == 0 ||
	    (perm & PTE_U) == 0 ||
	    (perm & PTE_COW) == 0)
		panic("not PTE_COW error. Dying");

	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.
	//   No need to explicitly delete the old page's mapping.
	perm = (perm | PTE_W) & ~PTE_COW;

	if ((r = sys_page_alloc(0, PFTEMP, perm)) != 0)
		panic("allocation fail %e", r);

	memcpy(PFTEMP, addr, PGSIZE);
	//cprintf("after memcpy\n");
	if ((r = sys_page_map(0, PFTEMP, 0, addr, perm)) != 0)
		panic("allocation fail %e", r);

	if ((r = sys_page_unmap(0, PFTEMP)) != 0)
		panic("unmap fail %e", r);

	return;
}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why do we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
//
int
duppage(envid_t envid, unsigned pn)
{
	int r;
	pte_t pte;
	int perm = 0;

	// LAB 4: Your code here.
	if (uvpd[pn >> 10] == 0)
		return 0;

	pte = uvpt[pn];
	perm = pte & 0xfff;

	if (pte == 0 ||
	    (perm & PTE_P) == 0)
		//Nothing to copy
		return 0;

	//cprintf("dup3 ustacktop %p addr = %p perm = %p\n", USTACKTOP, pn * PGSIZE, perm);
	if ((perm & PTE_COW) != 0 &&
	    (perm & PTE_SHARE) != 0)
		return -E_INVAL;

	// Share the page!
	if ((perm & PTE_SHARE) != 0) {
		r = sys_page_map(0, (void *) (pn * PGSIZE), envid,
		    (void *) (pn * PGSIZE), perm);
		if (r != 0)
			panic("can't map page");

		return 0;
	}

	if ((perm & PTE_W) != 0) {
		perm |= PTE_COW;
		perm &= ~PTE_W;
	}

	r = sys_page_map(0, (void *) (pn * PGSIZE), envid,
	    (void *) (pn * PGSIZE), perm);
	if (r != 0)
		panic("can't map page");

	//cprintf("dup5 perm = %p\n", perm);
	//mark page as PTE_COW
	if (perm & PTE_COW) {
		r = sys_page_map(0, (void *) (pn * PGSIZE), 0,
		    (void *) (pn * PGSIZE), perm);
		if (r != 0)
			panic("can't map page");
	}

	return 0;
}

extern void _pgfault_upcall();
//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use uvpd, uvpt, and duppage.
//   Remember to fix "thisenv" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	// LAB 4: Your code here.
	envid_t envid;
	uint32_t addr;
	uint32_t perm;
	int r;

	set_pgfault_handler(pgfault);

	envid = sys_exofork();
	if (envid < 0)
		panic("sys_exofork: %e", envid);
	if (envid == 0) {
		// We're the child.
		// The copied value of the global variable 'thisenv'
		// is no longer valid (it refers to the parent!).
		// Fix it and return 0.
		thisenv = &envs[ENVX(sys_getenvid())];

		return 0;
	}

	//cprintf("before duppage\n");

	for (addr = 0; addr < USTACKTOP; addr += PGSIZE) {
		duppage(envid, addr / PGSIZE);
	}
	//cprintf("after duppage\n");

	//cprintf("uvpd[UXSTACKTOP - 1] = %p\n", uvpd[(UXSTACKTOP - PGSIZE) >> 22]);

	perm = uvpt[UXSTACKTOP/PGSIZE - 1] & 0xfff; 
	if ((r = sys_page_alloc(0, PFTEMP, perm)) != 0)
		panic("allocation fail %e", r);

	memcpy(PFTEMP, (void *) UXSTACKTOP - PGSIZE, PGSIZE);

	if ((r = sys_page_map(0, PFTEMP, envid, (void *)(UXSTACKTOP - PGSIZE), perm | PTE_W)) != 0)
		panic("page_map fail %e", r);

	if ((r = sys_page_unmap(0, PFTEMP)) != 0)
		panic("unmap fail %e", r);

	if ((r = sys_env_set_status(envid, ENV_RUNNABLE)) < 0)
		panic("sys_env_set_status: %e", r);

	sys_env_set_pgfault_upcall(envid, _pgfault_upcall);

	return envid;
}

// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
