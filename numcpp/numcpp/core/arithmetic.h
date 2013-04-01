#ifndef NUMCPP_ARITHMETIC_H
#define NUMCPP_ARITHMETIC_H

namespace numcpp
{

#define _DEFINE_NUMCPP_BINARY_OPERATOR(OP,OP_NAME)             \
template <class Op1, class Op2> \
class Array_ ## OP_NAME : public  AbstractArray<decltype(std::declval<typename Op1::value_type>() OP std::declval< typename Op2::value_type>() ), Op1::dim, Array_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<typename Op1::value_type>() OP std::declval< typename Op2::value_type>() ) value_type; \
  Array_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return op1_[i] OP op2_[i]; \
  } \
  template<class...A> \
  typename std::enable_if< !isSlicedArray< A... >::value, \
        value_type \
        >::type \
  operator()(A...args) const \
  { \
    return op1_(args...) OP op2_(args...); \
  } \
  \
  size_t size() const \
  { \
    return op1_.size(); \
  } \
  \
  const std::array<size_t,Op1::dim>& shape() const \
  { \
    return op1_.shape(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class U, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractArray<T, Lhs::dim, Lhs>& lhs, const AbstractArray<U, Rhs::dim, Rhs>& rhs) \
    -> Array_ ## OP_NAME < AbstractArray<T, Lhs::dim, Lhs>, AbstractArray<U, Rhs::dim, Rhs> > \
{ \
    return Array_ ## OP_NAME < AbstractArray<T, Lhs::dim, Lhs>, AbstractArray<U, Rhs::dim, Rhs> > (lhs, rhs); \
} \
\
template <class Op1, class Op2> \
class Array_scalar_left_ ## OP_NAME : public  AbstractArray<decltype(std::declval<Op1>() OP std::declval< typename Op2::value_type>() ), Op2::dim, Array_scalar_left_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<Op1>() OP std::declval< typename Op2::value_type>() ) value_type; \
  Array_scalar_left_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return op1_ OP op2_[i]; \
  } \
  \
  size_t size() const \
  { \
    return op2_.size(); \
  } \
  \
  const std::array<size_t,Op2::dim>& shape() const \
  { \
    return op2_.shape(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class Lhs, class Rhs> \
auto operator OP \
    (const Lhs& lhs, const AbstractArray<T,Rhs::dim,Rhs>& rhs) \
    -> typename std::enable_if<std::is_arithmetic<typename complexBaseType<Lhs>::type>::value, \
        Array_scalar_left_ ## OP_NAME < Lhs, AbstractArray<T, Rhs::dim, Rhs> > \
        >::type  \
{ \
    return Array_scalar_left_ ## OP_NAME < Lhs, AbstractArray<T,Rhs::dim, Rhs> > (lhs, rhs); \
} \
\
template <class Op1, class Op2> \
class Array_scalar_right_ ## OP_NAME : public  AbstractArray<decltype(std::declval<typename Op1::value_type>() OP std::declval<Op2>()), Op1::dim, Array_scalar_right_ ## OP_NAME <Op1,Op2>   > \
{ \
public: \
  typedef decltype(std::declval<typename Op1::value_type>() OP std::declval<Op2>()) value_type; \
  Array_scalar_right_ ## OP_NAME (const Op1& a, const Op2& b) \
    : op1_(a), op2_(b) { } \
  \
  value_type operator[] (size_t i) const \
  { \
    return op1_[i] OP op2_; \
  } \
  \
  size_t size() const \
  { \
    return op1_.size(); \
  } \
  \
  const std::array<size_t,Op1::dim>& shape() const \
  { \
    return op1_.shape(); \
  } \
public: \
    const Op1& op1_; \
    const Op2& op2_; \
}; \
\
template <class T, class Lhs, class Rhs> \
auto operator OP \
    (const AbstractArray<T,Lhs::dim,Lhs>& lhs, const Rhs& rhs) \
    -> typename std::enable_if<std::is_arithmetic<typename complexBaseType<Rhs>::type>::value, \
        Array_scalar_right_ ## OP_NAME < AbstractArray<T,Lhs::dim, Lhs>, Rhs > \
        >::type  \
{ \
    return Array_scalar_right_ ## OP_NAME < AbstractArray<T,Lhs::dim, Lhs>, Rhs > (lhs, rhs); \
}


_DEFINE_NUMCPP_BINARY_OPERATOR(+,plus)
_DEFINE_NUMCPP_BINARY_OPERATOR(-,minus)
_DEFINE_NUMCPP_BINARY_OPERATOR(*,mult)
_DEFINE_NUMCPP_BINARY_OPERATOR(/,div)

#undef _DEFINE_NUMCPP_BINARY_OPERATOR


}

#endif
