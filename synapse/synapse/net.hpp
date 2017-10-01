#include "adiff.hpp"
#include "matrix.hpp"
#include <random>

namespace synapse {
  template<typename T>
  class net_base {
  protected:
    std::vector<T> vars;
    typename std::vector<T>::iterator it;
  public:
    using iterator = typename std::vector<T>::iterator;
    void begin() { it = vars.begin(); }
  };
    
  template<typename T>
  class net : public net_base<adiff<T>> {
  private:
    std::random_device rd;  
    std::mt19937 gen;
    std::normal_distribution<T> dist;
  public:
    net(float stddev = 0.1) : gen(rd()), dist(0, stddev) {}
    template<int rows, int cols>
    auto mat() -> decltype(make_view_matrix<rows, cols>(make_view(this->vars.begin(), this->vars.end()))) {
      auto start = this->it;
      this->it += rows*cols;
      if(this->it>=this->vars.end()) {
        this->vars.resize(std::distance(this->vars.begin(), this->it)-1);
        int i = std::distance(this->vars.begin(), start);
        std::generate(start, this->it, [&](){ return adiff<T>{dist(gen), { {i++, 1} } }; });
      }
      return make_view_matrix<rows, cols>(make_view(start, this->it));
    }
    void train(adiff<T>& loss) {
      std::sort(loss.ders.begin(), loss.ders.end());
      auto vit = this->vars.begin();
      auto lit = loss.ders.begin();
      for(int i=0; vit!=this->vars.end(); ++vit, ++lit, ++i) {
        assert(lit->index == i);
        *vit -= lit->der;
      }
    }
  };
  /*
  template<typename T>
  class neural_net {
  private:
    std::vector<T> vars;
    typename std::vector<T>::iterator it;
  public:
    void begin() { it = vars.begin(); }
    T var(const T& val) {
      if(it==vars.end()) {
        vars.push_back(val);
      }
      return *it++;
    }
    T var() {
      if(it==vars.end()) {
        vars.push_back(T());
      }
      return *it++;
    }
    adiff<T> train_var(const T& val) {
      return adiff<T>{ var(val), std::make_pair(vars.size()-1, 1) };
    }
    adiff<T> train_var() {
      return adiff<T>{ var(), std::make_pair(vars.size()-1, 1) };
    }
    template<int rows, int cols>
    matrix<T> mat(const matrix<T, rows, cols>& m) {
      if(it==vars.end()) {
        vars.reserve(vars.size()+rows*cols);
        vars.insert(vars.end(), m.data.begin(), m.data.end());
      }
      auto start = it;
      it += rows*cols;
      return { std::array<T>(start, vars.end()) };
    }
    template<int rows, int cols>
    matrix<T> mat() {
      auto start = it;
      it += rows*cols;
      return { std::array<T>(start, vars.end()) };
    }
    template<int rows, int cols>
    matrix<adiff<T>, rows, cols> train_mat(const matrix<T, rows, cols>& m) {
      if(it==vars.end()) {
        vars.reserve(vars.size()+rows*cols);
        vars.insert(vars.end(), m.data.begin(), m.data.end());
      }
      std::array<adiff<T>, rows*cols> a;
      int i = std::distance(vars.begin(), it);
      std::transform(it, it+a.size(), a.data.begin()+1, [](const T& val) { return { val, {i++, 1} }; });
      it += a.size();
      return { a };
    }
      
    template<int rows, int cols>
    matrix<adiff<T>, rows, cols> train_mat() {
      std::array<adiff<T>, rows*cols> a;
      int i = std::distance(vars.begin(), it);
      std::transform(it, it+a.size(), a.data.begin()+1, [](const T& val) { return { val, {i++, 1} }; });
      it += a.size();
      return { a };
    }
  
    void train(const adiff<T>& grad) {
      for(auto& var : vars) 
        var -= grad.der[var.index];
    }
  };*/
};

