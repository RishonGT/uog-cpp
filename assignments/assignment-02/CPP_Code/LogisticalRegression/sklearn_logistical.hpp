#ifndef SKLEARN_LOGISTICAL_HPP
#define SKLEARN_LOGISTICAL_HPP

#include <vector>
#include <cmath>
#include <numeric>

namespace linear_model
{
class LogisticalRegression
{
private:

    std::vector<double> m_weights;
    double m_bias{0.0};
    double m_learningrate{0.01};
    double m_lambda{0.01};
    
    //dot product of x.w
    double dot(const std::vector<double> &x) const
    {
        double result {0.0};
        for(auto i{0}; i<x.size(); ++i)
        {
            result += x[i] * m_weights[i];
        }
        return result;
    }

    //sigmoid function
    double sigmoid(const double &z) const
    {
        double y_hat {1/(1+(exp(-z)))};
        return y_hat;
    }

    //prediction or y_hat
    double predict(const std::vector<double> &x) const
    {
        double z {dot(x) + m_bias};
        return sigmoid(z);
    }

    //cross entropy loss
    double xentropyLoss(
        const std::vector<double> &x,
        const std::vector<double> &y
    ) const
    {
        double loss {0.0};
        for(auto i{0}; i < x.size(); ++i)
        {
            double y_hat = predict(x);
            loss += y[i] * log(y_hat) + (1- y[i]) * log(1 - y_hat);
        }
        loss = -loss / y.size();
        
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

    //TODO: Training the data
    

public:
    //Constructor
    LogisticalRegression(int features, double learningrate)
    :m_learningrate{learningrate}
{
    m_weights.resize(features,0.0);
}

};
}

#endif