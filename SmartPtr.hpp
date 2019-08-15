
#ifndef SMARTPTR_H
#define SMARTPTR_H
#include <map>
#include <iterator>
/*
template <class T> class Pointer
    {
        T *pointer;
        int count;
        Pointer()
        {
            pointer = nullptr;
            count = 0;
        }
        void DestroyPtr()
        {
            if (count)
            count--;
            //printf("~sharedPtr: count = %d\n", count);
            if (count == 0)
            {
                if (pointer != nullptr)
                {
                   delete pointer;
                   ~Pointer();
                }
                printf("~Pointer: pointer deleted!!!\n");
            }
        }
        ~Pointer()
        {
        }

    Pointer<T> p;
*/

template <class T> class sharedPtr
{
    T *pointer;
    int count;
    static std::map<T *, int> pointer_map;
    //T *pointer;
    //static int count;
    public:
    sharedPtr()
    {
        pointer = nullptr;
    }
    sharedPtr(T *p)
    {
        count = 1;
        pointer = p;
        pointer_map.insert(std::make_pair(pointer, count));
    }

    sharedPtr(const sharedPtr<T>& rhs)
    {
        auto found_1 = pointer_map.find(rhs.pointer);
        //if (this->pointer != nullptr)
        auto found_2 = pointer_map.find(this->pointer);
        if (found_1 == found_2)
            return ;
        if (found_1 != pointer_map.end())
        {
            count = ++found_1->second;
            pointer = found_1->first;

        }
        if (found_2 != pointer_map.end())
        {
            --found_2->second;
            if (found_2->second == 0)
            {
                pointer_map.erase(found_2->first);
            }
        }
        //printf("sharedPtr(T const& rhs) invoked\n");
    }

    sharedPtr<T>& operator= (const sharedPtr<T>& rhs)
    {
        auto found_1 = pointer_map.find(rhs.pointer);
        //if (this->pointer != nullptr)
        auto found_2 = pointer_map.find(this->pointer);
        if (found_1 == found_2)
            return *this;
        if (found_1 != pointer_map.end())
        {
            count = ++found_1->second;
            pointer = found_1->first;

        }
        if (found_2 != pointer_map.end())
        {
            --found_2->second;
            if (found_2->second == 0)
            {
                pointer_map.erase(found_2->first);
            }
        }
        return *this;
    }


    ~sharedPtr()
    {
        auto found_1 = pointer_map.find(pointer);
        if (found_1->second == 1)
        {
            pointer_map.erase(found_1->first);
            delete pointer;
        }
        else
            found_1->second--;
        
    }

    int use_count()
    {
         auto found_1 = pointer_map.find(pointer);
        return found_1->second;
    }
    T *get()
    {
        auto found_1 = pointer_map.find(pointer);
        return found_1->first;
    }
};

template <class T>
std::map<T *, int> sharedPtr<T>::pointer_map;

template <class T> class uniquePtr
{

};

#endif