#include <type_traits>

namespace synapse {
    
  struct expr_base {};
    
  template<>
  struct arglist {
    
  };
    
  class variable : public expr_base {
    int index;
  public:
    variable() : val(val), index(0) {}
    void set_index(int i) { index = i; }
    template<typename T>
    T operator()(const T& val) const { return }
  };
    
  template<typename T>
  class literal : public expr_base {
    const T val;
  public:
    literal(const T& val) : val(val) {}
    T eval() const { return val; }
  };
    
  template<typename Op, typename... Es>
  class expression;
    
  template<typename E> 
  struct expr_traits {
    using type = E;
  };

  template<typename T>
  struct expr_traits<std::enable_if<std::is_pod<T>::value, T>> {
    using type = literal<T>;  
  };
    
  template<typename UnaryOp, typename E>
  class expression<UnaryOp, E> : public expr_base {
    UnaryOp op;
    typename expr_traits<E>::type e;
  public:
    expression(const UnaryOp& op, const E& e) : op(op), e(e) {}
    auto eval() const { return op(e.eval()); }
  };
    
  template<typename BinaryOp, typename E1, typename E2>
  class expression<BinaryOp, E1, E2> : public expr_base {
    BinaryOp op;
    typename expr_traits<E1>::type e1;
    typename expr_traits<E2>::type e2;
  public:
    expression(const BinaryOp& op, const E1& e1, const E2& e2) : op(op), e1(e1), e2(e2) {}
    auto eval() const { return op(e1.eval(), e2.eval()); }
  };
    
  template<typename Op, typename... Es>
  auto make_expression(const Op& op, Es... es) {
    return expression<Op, Es...>(op, es...);
  }
    
  struct plus {
    template<typename T1, typename T2>
    auto operator()(const T1& v1, const T2& v2) const { return v1 + v2; }
  };
    
  template<typename E1, typename E2>
  typename std::enable_if<std::is_convertible<E1, expr_base>::value or std::is_convertible<E2, expr_base>::value,
    expression<plus, E1, E2>>::type operator+(const E1& e1, const E2& e2) {
    return make_expression(plus(), e1, e2);
  }
  
};