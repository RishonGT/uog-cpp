#include "sklearn_logistical.hpp"

#include <vector>
#include <numeric>
#include <cmath>

namespace linear_model
{

LogisticalRegression::LogisticalRegression(int features, double learningrate)
    :m_learningrate{learningrate}
{
    m_weights.resize(features,0.0);
};

double LogisticalRegression::dot(const std::vector<double> &x) const
{
    //dot product of x.w
    double result {0.0};
    //size_t cause .size() returns whatever that is
    for(size_t i{0}; i<x.size(); ++i)
    {
        result += x[i] * m_weights[i];
    }

    return result;
}

double LogisticalRegression::sigmoid(const double &z) const
{
    double y_hat {1/(1+(exp(-z)))};
    return y_hat;
}

double LogisticalRegression::predict(const std::vector<double>  &X) const
{
    double z {dot(X) + m_bias};
    return sigmoid(z);
}

double LogisticalRegression::xentropyLoss(
    const std::vector<double> &X,
    const std::vector<double> &Y
) const
{
    double loss {0.0};
    for(auto i{0}; i < X.size(); ++i)
    {
        double y_hat = predict(X);
        loss += Y[i] * log(y_hat) + (1- Y[i]) * log(1 - y_hat);
    }
    loss = -loss / Y.size();
    
    //Regularization
    //Sum of Weights
    double weights{0.0};
    for(double w: m_weights)
    {
        weights += w * w;
    }
    loss += m_lambda * weights;

    //Bias term
    loss += m_lambda * m_bias * m_bias;
    
    return loss;
}
}

