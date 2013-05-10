#include <Eigen/Core>
#include <numcpp.h>

using namespace numcpp;

void performance03NumCpp(size_t N, size_t M)
{
    Vector<float> x = randn<float>(N);
    Matrix<float> A = randn<float>(N,N);
    Vector<float> y(N);


    auto t = tic();

    //EnableCaching();

    for(auto i=0; i<M; i++)
    {
      y += dot(A,x)*i; // + dot(A,ones(N))*i;
    }

    //DisableCaching();

    toc(t);
}

void performance03Eigen(size_t N, size_t M)
{
    Eigen::VectorXf x = Eigen::VectorXf::Random(N);
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(N,N);
    Eigen::VectorXf y(N);

    auto t = tic();
    for(auto i=0; i<M; i++)
    {
      y += A*x*i; //; + A*Eigen::VectorXf::Ones(N)*i;
    }

    toc(t);

}

int main()
{
  for(int l=6; l<12; l++)
  {
    auto N = pow(2,l);
    auto M = N;
    std::cout << "NumCpp\n";
    performance03NumCpp(N,M);
    std::cout << "Eigen\n";
    performance03Eigen(N,M);
    std::cout << "\n";
  }

  return 0;
}