#ifndef SKLEARN_LOGISTICAL_HPP
#define SKLEARN_LOGISTICAL_HPP

#include <vector>

namespace linear_model
{
class LogisticalRegressionMulticlass
{
private:

    std::vector<double> m_weights;
    std::vector<double> n_weights;
    double m_bias{0.0};
    double m_learningrate{0.01};
    double m_lambda{0.01};
    
    //dot product of x.w
    
    double sigmoid(const double &z) const;
    //prediction or y_hat



    //TODO: Gradient descent
    //TODO: Training the data

public:
    //Constructor
    LogisticalRegressionMulticlass(int features, double learningrate, int bias);
    std::vector<double>& getWeights() 
    {
        return m_weights;
    }
    double& getBias() 
    {
        return m_bias;
    }
    double dot(const std::vector<double> &X) const;
    double predict(const std::vector<double> &x) const;
    //cross entropy loss
    double xentropyLoss(
        const std::vector<double> &X,
        const std::vector<double> &Y,
        const int img_count
    ) const;
    double gradientDescent(
        const std::vector<double> &X,
        const std::vector<double> &Y,
        const int img_count
    ) const;
    double gradientUpdate(
        const std::vector<double> &X,
        const std::vector<double> &Y
    ) const;



};
}

#endif