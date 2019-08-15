
#ifndef SMARTPTR_H
#define SMARTPTR_H

template <class T> class sharedPtr
{
    T *pointer;
    static int count;
    public:
    sharedPtr()
    {
        pointer = nullptr;
    }
    sharedPtr(T *p)
    {
        if (!pointer)
            count = 1;
        else
            count++;
        pointer = p;
    }

    sharedPtr(const sharedPtr<T>& rhs)
    {
        //printf("sharedPtr(T const& rhs) invoked\n");
        if (!rhs.pointer)
        {
            pointer = nullptr;
            return ;
        }
        //if (rhs.pointer != this->pointer)
        //{
        //}   
        count++;
        rhs.count = count;
    }

    sharedPtr<T>& operator= (const sharedPtr<T>& rhs)
    {
        //printf("T& operator= (T const& rhs) invoked\n");
        if (!rhs.pointer)
            return *this;
        if (rhs.pointer != this->pointer)
            pointer = rhs.pointer;
        count = ++rhs.count;
        return *this;
    }


    ~sharedPtr()
    {
        if (count)
            count--;
        //printf("~sharedPtr: count = %d\n", count);
        if (count == 0)
        {
            if (pointer != nullptr)
                delete pointer;
            printf("~sharedPtr: pointer deleted!!!\n");
        }
    }

    int use_count()
    {
        return count;
    }
    T *get()
    {
        return pointer;
    }
};

template <class T>
int sharedPtr<T>::count = 0;

template <class T> class uniquePtr
{

};

#endif