#include <array>
#include <memory>
#include <algorithm>
#include <initializer_list>
#include <cassert>
#include <ostream>

namespace synapse {
    
  template<typename Iterator>
  class view {
  private:
    Iterator _begin, _end;
  public:
    using iterator = Iterator;
    view(const iterator& begin, const iterator& end) {
      _begin = begin; 
      _end = end;
    }
    iterator begin() { return _begin; }
    iterator end() { return _end; }
    std::size_t size() const { return std::distance(_begin, _end); }
  };
  
  template<typename Iterator>
  view<Iterator> make_view(const Iterator& begin, const Iterator& end) {
    return view<Iterator>(begin, end);
  }
  
  template<typename T, int rows, int cols, typename Container>
  class matrix_base {
  protected:
    Container data;
    matrix_base() {}
  public:
    using iterator = typename Container::iterator;
    static std::size_t size() { return rows*cols; }
    matrix_base(const Container& c) : data(c) { assert(c.size() == size()); }
    template<typename C>
    matrix_base& operator=(C& c) {
      assert(c.size() == size());
      std::copy(c.begin(), c.end(), data.begin());
      return *this;
    }
    template<typename T2>
    matrix_base& operator=(std::initializer_list<T2>& l) {
      assert(l.size() == size());
      std::copy(l.begin(), l.end(), data.begin());
      return *this;
    }
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const T& operator[](const std::size_t i) const { return data[i]; }
    T& operator[](const std::size_t i) { return data[i]; }
    const T& operator()(const int r, const int c) const { return data[r*cols+c]; }
    T& operator()(const int r, const int c) { return data[r*cols+c]; }
  };
    
  template<typename T, int rows, int cols, typename Container>
  std::ostream& operator<<(std::ostream& os, matrix_base<T, rows, cols, Container>& m) {
    int c = 0;
    for(auto it=m.begin(); it!=m.end();) {
      for(int c=0; c<cols; ++c, ++it)
        os << *it << " ";
      os << "\n";
    }
    return os;
  }
    
  template<typename T, int rows, int cols>
  class matrix : public matrix_base<T, rows, cols, std::array<T, rows*cols>> {
  public:
    using iterator = typename std::array<T, rows*cols>::iterator;
    matrix() {}
    template<typename C>
    matrix(C c) {
      assert(c.size() == this->size());
      std::copy(c.begin(), c.end(), this->begin());
    }
    template<typename T2>
    matrix(std::initializer_list<T2> l) {
      assert(l.size() == this->size());
      std::copy(l.begin(), l.end(), this->begin());
    }
  };
    
  template<int rows, int cols, typename Iterator>
  matrix<typename Iterator::value_type, rows, cols> make_view_matrix(const view<Iterator>& v) {
    return matrix_base<typename Iterator::value_type, rows, cols, view<Iterator>>(v);
  }
  
  template<typename T, int rows>
  using vector = matrix<T, rows, 1>;
  
  template<typename T1, typename T2, int rows, int same>
  auto operator*(matrix<T1, rows, same>& lhs, vector<T2, same>& rhs)
    -> vector<decltype(T1()*T2()), rows> {
      vector<decltype(T1()*T2()), rows> result;
      auto lit = lhs.begin();
      for(auto resit = result.begin(); resit!=result.end(); ++resit) {
        for(auto rit=rhs.begin(); rit!=rhs.end(); ++lit, ++rit)
          if(rit == rhs.begin())
            *resit = (*lit) * (*rit);
          else
            *resit += (*lit) * (*rit);
      }
      return result;
    }
};
