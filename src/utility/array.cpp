//
//  array.cpp
//  Feynman_Simulator
//
//  Created by Kun Chen on 1/18/15.
//  Copyright (c) 2015 Kun Chen. All rights reserved.
//

#include <algorithm>
#include "array.h"
#include "utility/abort.h"
#include "utility/dictionary.h"

template <uint DIM>
Array<DIM>::Array(const Array& source)
{
    this->Allocate(source.GetShape(), source.Data());
}

template <uint DIM>
Array<DIM>& Array<DIM>::operator=(const Complex& c)
{
    ASSERT_ALLWAYS(_Data != nullptr, "Array should be allocated first!");
    for (uint i = 0; i < _Size; i++)
        _Data[i] = c;
    return *this;
}
template <uint DIM>
Array<DIM>& Array<DIM>::operator=(const Array& source)
{
    if (&source == this)
        return *this;
    *this = source.Data();
    return *this;
}
template <uint DIM>
Array<DIM>& Array<DIM>::operator=(const Complex* source)
{
    if (_Data == source)
        return *this;
    std::copy(source, source + _Size, _Data);
    return *this;
}

template <uint DIM>
const uint* Array<DIM>::GetShape() const
{
    return _Shape;
}

template <uint DIM>
Complex* Array<DIM>::Data() const
{
    return _Data;
}

template <uint DIM>
void Array<DIM>::Allocate(const uint* Shape_, const Complex* data)
{
    ASSERT_ALLWAYS(_Data == nullptr, "Please free Array first!");
    std::copy(Shape_, Shape_ + DIM, _Shape);
    _Size = 1;
    for (uint i = 0; i < DIM; i++) {
        _Cache[DIM - 1 - i] = _Size;
        _Size *= _Shape[i];
    }
    _Data = new Complex[_Size];
    if (_Data == nullptr)
        THROW_ERROR(MemoryException, "Fail to allocate array!");
    if (data != nullptr)
        std::copy(data, data + DIM, _Data);
}

template <uint DIM>
void Array<DIM>::Free()
{
    delete[] _Data;
    _Data = nullptr;
}

template class Array<1>;
template class Array<2>;
template class Array<3>;
template class Array<4>;
template class Array<5>;

void test()
{
    Array<2> a;
    uint shape[2] = { 1, 2 };
    a.Allocate(shape);
    a(shape);
}
