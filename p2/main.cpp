#include <boost/container/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

using namespace boost::interprocess;

// Определить типы, которые мы будем использовать
typedef managed_shared_memory::segment_manager SegmentManager;
typedef allocator<char, SegmentManager> CharAllocator;
typedef boost::container::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;
typedef allocator<ShmString, SegmentManager> ShmemAllocator;
typedef boost::container::vector<ShmString, ShmemAllocator> MyVector;

int main() {
    try {
        // Открываем сегмент
        managed_shared_memory segment(open_only, "MySharedMemory");
        named_mutex mutex(open_only, "mutex");
        // Находим вектор, используя имя строки
        MyVector* myvector = segment.find<MyVector>("MyVector").first;

        // Выводим список
        mutex.lock();
        for (const auto& item : *myvector) {
            // Преобразуем ShmString обратно в std::string для std::cout
            std::string str(item.begin(), item.end());
            std::cout << str << std::endl;
        }
        mutex.unlock();

        // Завершив, уничтожаем вектор из сегмента
        //        segment.destroy<MyVector>("MyVector");

        // Также удаляем общую память
        //        shared_memory_object::remove("MyName");
    } catch (...) {
        // Если открыть сегмент или найти вектор не удалось, тогда также удаляем общую память
        shared_memory_object::remove("MyName");
    }

    return 0;
}