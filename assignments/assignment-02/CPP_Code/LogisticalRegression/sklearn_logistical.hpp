#ifndef SKLEARN_LOGISTICAL_HPP
#define SKLEARN_LOGISTICAL_HPP

#include <vector>

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
    double dot(const std::vector<double> &X) const;
    //sigmoid function
    double sigmoid(const double &z) const;
    //prediction or y_hat
    double predict(const std::vector<double> &x) const;
    //cross entropy loss
    double xentropyLoss(
        const std::vector<double> &X,
        const std::vector<double> &Y
    ) const;

    //TODO: Gradient descent
    //TODO: Training the data

public:
    //Constructor
    LogisticalRegression(int features, double learningrate);


};
}

#endif