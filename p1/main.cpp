#include <boost/container/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace boost::interprocess;

typedef managed_shared_memory::segment_manager SegmentManager;
typedef allocator<char, SegmentManager> CharAllocator;
typedef boost::container::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;
typedef allocator<ShmString, SegmentManager> ShmemAllocator;
typedef boost::container::vector<ShmString, ShmemAllocator> MyVector;

int main() {
    shared_memory_object::remove("MySharedMemory");
    named_mutex::remove("mutex");
    named_mutex mutex(open_or_create, "mutex");
    managed_shared_memory segment(create_only, "MySharedMemory", 65536);

    ShmemAllocator strAlloc(segment.get_segment_manager());
    ShmString shmString(strAlloc);
    shmString = "Hello, Shared Memory!";
    MyVector* myVector = segment.construct<MyVector>("MyVector")(strAlloc);
    size_t count = 10;
    while (true) {
        mutex.lock();
        for (auto i = 0u; i < count; i++) {
            ShmString tmp_str(strAlloc);
            tmp_str = shmString;
            std::string postfix = std::to_string(i);
            tmp_str.append(shmString.data());
            tmp_str.append(postfix.data());
            myVector->push_back(tmp_str);
        }
        mutex.unlock();
        sleep(5);
        count++;
    }
    return 0;
}