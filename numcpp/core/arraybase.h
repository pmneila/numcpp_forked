
#ifndef NUMCPP_ARRAYBASE_H
#define NUMCPP_ARRAYBASE_H

#include <algorithm>

#include "arraycore.h"
#include "abstractexpression.h"
#include "index.h"

namespace numcpp
{

template<class T> class Array;
template<class T> class ArrayRef;
template<typename T> class Iterator;

template <typename T, typename Derived>
class ArrayBase : public AbstractArrayExpression<T, Derived>
{
protected:
    unsigned char* _data;
    ArrayCore _core;
    
    ArrayBase(const ArrayBase& rhs)
        : _data(rhs._data)
        , _core(rhs._core)
    {}
    
    ArrayBase(const ArrayCore& core)
        : _data(reinterpret_cast<unsigned char*>(core.data()))
        , _core(core)
    {}
    
    const ArrayBase& operator=(const ArrayBase& rhs)
    {
        _data = rhs._data;
        _core = rhs._core;
    }
    
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef Iterator<T> iterator;
    typedef Iterator<const T> const_iterator;
    
    T& operator[](const std::vector<size_t>& index)
    {
        std::ptrdiff_t offset = _core.offset(index);
        return *reinterpret_cast<T*>(_data + offset);
    }
    
    ArrayRef<T> operator[](std::vector<Index> index)
    {
        std::ptrdiff_t newOffset = _core.offset();
        
        Strides newStrides;
        Shape newShape;
        
        auto shape_it = shape().begin();
        auto strides_it = strides().begin();
        for(auto index_it = index.begin(); index_it != index.end();
            ++index_it)
        {
            int start;
            
            if(index_it->isNewAxis())
            {
                newShape.push_back(1);
                newStrides.push_back(0);
                continue;
            }
            
            if(shape_it == shape().end())
                throw std::invalid_argument("the index has more elements than array dimensions");
            
            if(index_it->isSingleton())
            {
                int index = index_it->index();
                start = index;
            }
            else
            {
                const Slice& slice = index_it->slice();
                start = slice.start(*shape_it);
                int end = slice.end(*shape_it);
                int step = slice.step();
                
                if(step > 0)
                    newShape.push_back(ceil_div(end - start, step));
                else
                    newShape.push_back(ceil_div(start - end, -step));
                
                newStrides.push_back(step * *strides_it);
            }
            
            newOffset += start * *strides_it;
            
            ++shape_it;
            ++strides_it;
        }
        
        for(; shape_it != shape().end(); ++shape_it, ++strides_it)
        {
            newShape.push_back(*shape_it);
            newStrides.push_back(*strides_it);
        }
        
        return ArrayRef<T>(ArrayCore(newShape, newStrides, manager(), newOffset));
    }
    
    iterator begin();
    iterator end();
    
    int numElements() const {return _core.numElements();}
    const Shape& shape() const {return _core.shape();}
    const Strides& strides() const {return _core.strides();}
    int ndims() const {return _core.ndims();}
    Manager::Ptr manager() const {return _core.manager();}
    bool isEmpty() const {return _core.isEmpty();}
    
    T* data() const {return reinterpret_cast<T*>(_data);}
    std::ptrdiff_t offset() const {return _core.offset();}
    
    ArrayCore& core() {return _core;}
    const ArrayCore& core() const {return _core;}
};

}

#include "iterator.h"

namespace numcpp
{

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::iterator ArrayBase<T, Derived>::begin()
{
    return iterator(*this);
}

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::iterator ArrayBase<T, Derived>::end()
{
    iterator it(*this);
    it.goToEnd();
    return it;
}

}

#endif