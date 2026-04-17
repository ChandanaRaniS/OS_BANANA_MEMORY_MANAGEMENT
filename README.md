#  Banana Problem – Memory Management Lab

##  Student Details

* **Name:** Chandana Rani S
* **SRN:** PES2UG24AM044
* **Class:** Sem 4 (CSE-AIML)
* **Section:** A

---

##  Overview

This project explores key concepts of **Operating System Memory Management** through practical experiments and observations. It covers virtual memory, demand paging, page tables, copy-on-write, fragmentation, swapping, and thrashing.

---

##  Section-wise Summary

### 🔹 Section 0: Physical Memory

* Total frames = **524288**
* Formula used:
  `Frames = RAM / Page Size`

---

### 🔹 Section 1: Virtual Address Space

* Observed layout:
  `Text < Rodata < Data < BSS < Heap << Stack`
* Heap is **readable & writable (rw-p)**, not executable → improves security
* **ASLR** observed: addresses change on each execution

---

### 🔹 Section 2: Demand Paging

* `malloc()` allocates **virtual memory**, not physical memory
* Physical memory used only when accessed
* ~65536 page faults observed (for 256 MB)

---

### 🔹 Section 3: Page Tables

* Pages are mapped only when accessed
* Present bit:

  * `0` → not in memory
  * `1` → loaded into memory
* PFNs vary between runs due to dynamic allocation

---

### 🔹 Section 4: Copy-on-Write

* `fork()` does **not copy memory immediately**
* Read → no page faults
* Write → triggers copy-on-write (~32768 faults)
* Efficient for memory usage

---

### 🔹 Section 5: Shared Libraries

* Static binaries are larger than dynamic ones
* Shared libraries:

  * Code → shared
  * Data → not shared
* Permissions:

  * `r-xp` → code
  * `r--p` → read-only
  * `rw-p` → writable

---

### 🔹 Section 6: malloc Internals

* Small allocations → `brk()` (heap grows)
* Large allocations → `mmap()`
* Threshold ≈ **128 KB**
* mmap reduces fragmentation

---

### 🔹 Section 7: Fragmentation

* External fragmentation observed
* Free memory may exist but not usable (non-contiguous)
* Paging solves fragmentation

---

### 🔹 Section 8: Swapping

* Observed using `vmstat`
* `si` & `so` > 0 → active swapping
* Performance drops due to disk access

---

### 🔹 Section 9: Page Replacement

* Smaller working set (64 MB) faster than larger (512 MB)
* Linux uses **approximate LRU**
* Active & inactive page lists used

---

### 🔹 Section 10: Thrashing

* Performance drops when memory > 100%
* Heavy swapping observed
* OOM killer prevents system crash
* `vm.swappiness` controls swap behavior

---

## Key Learnings

* Virtual memory ≠ physical memory
* Demand paging improves efficiency
* Copy-on-write saves memory
* Fragmentation affects allocation
* Swapping and thrashing degrade performance

---

## 🛠️ Tools Used

* GCC Compiler
* Linux commands:

  * `vmstat`
  * `perf`
  * `/proc/self/maps`
  * `pmap`

---

##  Reference

Full report: 

---

##  Conclusion

This experiment provided a deep understanding of how operating systems manage memory efficiently using techniques like paging, sharing, and dynamic allocation.

---
