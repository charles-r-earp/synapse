#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cassert>

namespace synapse {
  template<typename T>
  struct derivative {
    int index;
    T der;
    template<typename T2>
    bool operator<(const derivative<T2>& d2) { return index < d2.index; }
    template<typename T2>
    bool operator>(const derivative<T2>& d2) { return index > d2.index; }
    template<typename T2>
    bool operator==(const derivative<T2>& d2) { return index == d2.index; }
  };
    
  template<typename T>
  struct adiff {
    T val;
    std::vector<derivative<T>> ders;
    T d(const int i) { 
      auto it = std::find(ders.begin(), ders.end(), derivative<T>{i, 0});
      assert(it != ders.end());
      return it->der;
    }
  };
    
  template<typename T1, typename T2, typename T3, typename F, typename D> 
  void op(adiff<T3>& result, const adiff<T1>& lhs, const adiff<T2>& rhs, const F& f, const D& df) {
    // f = f(lval, rval)
    // df = df(lval, lder, rval, rder)
    result.val = f(lhs.val, rhs.val);
    std::vector<derivative<T1>> lders(lhs.ders.begin(), lhs.ders.end());
    std::vector<derivative<T2>> rders(rhs.ders.begin(), rhs.ders.end());
    result.ders.clear();
    // partition lhs with matching derivatives first  
    auto lpartit = std::partition(lders.begin(), lders.end(), [&](const derivative<T2>& d2){
        return std::find(rders.begin(), rders.end(), d2) != rders.end();
    });
    // partition rhs with matching derivatives first              
    auto rpartit = std::partition(rders.begin(), rders.end(), [&](const derivative<T1>& d1){
        return std::find(lders.begin(), lders.end(), d1) != lders.end();
    });
    // same number of elements that match
    assert(std::distance(lders.begin(), lpartit) == std::distance(rders.begin(), rpartit));
    std::transform(
      lders.begin(), 
      lders.end(),
      rders.begin(),
      std::back_inserter(result.ders),
      [&](const derivative<T1>& d1, const derivative<T2>& d2) {
        assert(d1.index == d2.index);
        return derivative<T3>{d1.index, df(lhs.val, d1.der, rhs.val, d2.der)};            
    });
    result.ders.insert(result.ders.end(), lpartit, lders.end());
    result.ders.insert(result.ders.end(), rpartit, rders.end());
  }

  template<typename T1, typename T2, typename F, typename D> 
  auto op(const adiff<T1>& lhs, const adiff<T2>& rhs, const F& f, const D& df) -> adiff<decltype(f(T1(), T2()))> {
    adiff<decltype(f(T1(), T2()))> result;
    op(result, lhs, rhs, f, df);
    return result;
  }
    
  // add
    
  template<typename T1, typename T2>
  auto operator+(const adiff<T1>& lhs, const adiff<T2>& rhs) -> adiff<decltype(T1()+T2())> {
    return op(lhs, rhs, [](T1 lv, T2 rv){ return lv + rv; }, [](T1 lv, T1 ld, T2 rv, T2 rd){ return ld + rd; });
  }
  
  template<typename T1, typename T2>
  auto operator+(const T1& lhs, const adiff<T2>& rhs) -> adiff<decltype(T1()+T2())> {
    return { lhs + rhs.val, rhs.ders };
  }
  
  template<typename T1, typename T2>
  auto operator+(const adiff<T1>& lhs, const T2& rhs) -> adiff<decltype(T1()+T2())> {
    return { lhs.val + rhs, lhs.ders };
  }
  
  template<typename T1, typename T2>
  adiff<T1>& operator+=(adiff<T1>& lhs, const adiff<T2>& rhs) {
    op(lhs, lhs, rhs, [](T1 lv, T2 rv){ return lv + rv; }, [](T1 lv, T1 ld, T2 rv, T2 rd){ return ld + rd; });
    return lhs;
  }
  
  template<typename T1, typename T2>
  auto operator+=(const T1& lhs, const adiff<T2>& rhs) -> decltype(T1()+T2()) {
    return lhs + rhs.val;
  }
  
  template<typename T1, typename T2>
  auto operator+=(adiff<T1>& lhs, const T2& rhs) -> adiff<decltype(T1()+T2())> {
    lhs.val += rhs;
    return lhs.val;
  }
  
  // multiply
    
  template<typename T1, typename T2>
  auto operator*(const adiff<T1>& lhs, const adiff<T2>& rhs) -> adiff<decltype(T1()+T2())> {
    return op(lhs, rhs, [](T1 lv, T2 rv){ return lv * rv; }, [](T1 lv, T1 ld, T2 rv, T2 rd){ 
      return ld*rv + lv*rd; 
    });
  }
  
  template<typename T1, typename T2>
  auto operator*(const T1& lhs, const adiff<T2>& rhs) -> adiff<decltype(T1()+T2())> {
    adiff<decltype(T1()+T2())> result = rhs;
    result.val *= lhs;
    for(auto& der : result.ders)
      der.der *= lhs;
    return result;
  }
  
  template<typename T1, typename T2>
  auto operator*(const adiff<T1>& lhs, const T2& rhs) -> adiff<decltype(T1()+T2())> {
    adiff<decltype(T1()+T2())> result = lhs;
    result.val *= rhs;
    for(auto& der : result.ders)
      der.der *= rhs;
    return result;
  }
  
  template<typename T1, typename T2>
  adiff<T1>& operator*=(adiff<T1>& lhs, const adiff<T2>& rhs) {
    op(lhs, lhs, rhs, [](T1 lv, T2 rv){ return lv + rv; }, [](T1 lv, T1 ld, T2 rv, T2 rd){
      return ld*rv + lv*rd;
    });
    return lhs;
  }
  
  
};