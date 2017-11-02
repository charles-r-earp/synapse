#include <thrust/version.h>
#include <thrust/device_vector.h>
#include <iostream>

int main() {
  int major = THRUST_MAJOR_VERSION;
  int minor = THRUST_MINOR_VERSION;
  std::cout << "Thrust v" << major << "." << minor << std::endl;
  std::cout << "test_thrust pass" << std::endl;
  return 0;
}