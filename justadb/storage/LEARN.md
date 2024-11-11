# Storage Engine

# What kind of data I would be adding into it?
## MVP (to start with)
1. Tables
2. Indexes
3. Actual data (tuples)


### Supported data types
1. Tuple
2. Numbers -> Integers (no floats)
3. Strings  
4. Booleans

*Everything should be stored inside a tuple and tuple must have a table reference*

### Low Level Memory Architecture
- Won't rely on OS Paging (can't use MMAP (https://man7.org/linux/man-pages/man2/mmap.2.html))

#  F2023 #06 - Database Memory & Disk I/O Management (CMU Intro to Database Systems) 

## Buffer Pool Organization

## Page Table
* keeps track of pages that are currently in the memory (or in the buffer pool) 
* with **latches** to ensure thread safe access
(why not locks)

## Additional Metadata Per Page:
1. Dirty flag
2. Pin/Reference counter
3. Access tracking information (?)


-> Page Table -> Buffer Pool

Page table will be used to get a page from buffer pool

## Lock vs Latch


# LOCKs
* protects the logical contents from other txs
* held for txs duration
* need to be able to rollback changes


# LATCHs
* low level internal primitives
* protects things like page information? from other threads internally in the db system
* help for the particular **internal operation** duration
* NO ROLLBACK (not tx fun :p)

## Not to confuse with std::latch (https://en.cppreference.com/w/cpp/thread/latch)

The latch class is a downward counter of type std::ptrdiff_t which can be used to synchronize threads. The value of the counter is initialized on creation. Threads may block on the latch until the counter is decremented to zero. There is no possibility to increase or reset the counter, which makes the latch a single-use barrier. 

## Also can't use OS's mutex (this also have problems wow)

# Page Directory vs Page Table
**Page Directory** is the mapping from page ids to page locations in the *database files*. 
Must be stored on disk so that the db can find it on restart.

**Page Table** is the mapping from page ids to a copy of the page in buffer pool frames.
In memory structure that does not need to be stored on a disk

# Allocation Policies
## Global Policies
* Make decisions for active queries

## Local Policies
* Allocate frames to **specific queries**
- That means I would need to treat the quries as objects 



## Buffer Pool Optimizations
* Multiple Buffer Pools
* Pre-Fetching
* Scan Sharing
* Buffer Pool Bypass


## Multiple Buffer Pools 
There won't always be a single buffer pool
There are going to be multiple instances, per db buffer pool, per page type buffer pool

## Buffer Pools at Runtime
## How to maintin multiple buffer pools?

### Approach 1 - Object ID

Have an object identifier in records (tuples?) and then maintain a **mapping** from **objects** to specific **buffer pools**.

Example
```
Q1: GET RECORD #123
<ObjectID, PageID, SlotNum>
```
**ObjectID -> BufferPool ID**


### Approach 2 - Hashing
Hash the page id to select which buffer pool to access
HASH(123) % n (number of buffer pools) => required BufferPoolID
