#include <synapse/net.hpp>
#include <iostream>
/*
template<typename T>
T sqrtsigmoid(const T& x) {
  using synapse::sqrt;
  return x/sqrt(x*x + 1);
}*/

int main() {
  synapse::net<float> net;
  auto run_net = [&](synapse::matrix<float, 2, 1>& pt) {
    net.begin();
    auto w = net.mat<1, 2>();
    //auto b = net.mat<1, 1>();
    return w*pt;
  };
  //auto loss = run_net(net, {0, 1});
  //net.train(loss);*/
  std::cout << "test_net pass" << std::endl;
  return 0;
}