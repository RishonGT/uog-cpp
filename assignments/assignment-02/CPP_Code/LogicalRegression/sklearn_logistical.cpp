#include "sklearn_logistical.hpp"

#include <vector>
#include <numeric>
#include <cmath>

namespace linear_model
{

////////////////////////////////////////////////////////////////////////
//////////////////Multiclass Logistic Regression////////////////////////
////////////////////////////////////////////////////////////////////////





LogisticalRegressionMulticlass::LogisticalRegressionMulticlass(int features, double learningrate, int classes)
    :m_learningrate{learningrate}
{
    m_weights.resize(features, 0.0);
}
;

double LogisticalRegressionMulticlass::dot(const std::vector<double> &x) const
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

double LogisticalRegressionMulticlass::sigmoid(const double &z) const
{
    double y_hat {1/(1+(exp(-z)))};
    return y_hat;
}

double LogisticalRegressionMulticlass::predict(const std::vector<double>  &X) const
{   
    double z = dot(X) + m_bias;
    return sigmoid(z);
}

double LogisticalRegressionMulticlass::xentropyLoss(
    const std::vector<double> &X,
    const std::vector<double> &Y,
    const int img_count
) const
{
    double loss {0.0};
    const double y_hat = predict(X);
    loss = Y[img_count] * log(y_hat) + (1- Y[img_count]) * log(1 - y_hat);
    loss = -loss;
    //Regularization
    //Sum of Weights
    /*
    double weights{0.0};
    for(double w: m_weights)
    {
        weights += w * w;
    }
    loss += m_lambda * weights;

    loss += m_lambda * m_bias * m_bias;
    */
    return loss;
}
double LogisticalRegressionMulticlass::gradientDescent(
    const std::vector<double> &X,
    const std::vector<double> &Y,
    const int img_count
) const
{
    double gradient {0.0};
    double y_hat = predict(X);
    gradient = (y_hat - Y[img_count]);
    return gradient;
}
/*
double LogisticalRegressionMulticlass::gradientUpdate(
    const std::vector<double> &X,
    const std::vector<double> &Y
) const
{
    double gradient {gradientDescent(X, Y)};
    for(size_t i{0}; i < m_weights.size(); ++i)
    {
        const_cast<double&>(m_weights[i]) = m_weights[i] - m_learningrate * gradient;
    }
    const_cast<double&>(m_bias) = m_bias - m_learningrate * gradient;
    return 0.0;
}
    */

} // namespace linear_model

