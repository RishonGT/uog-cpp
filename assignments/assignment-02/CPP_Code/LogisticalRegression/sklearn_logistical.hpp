#ifndef SKLEARN_LOGISTICAL_HPP
#define SKLEARN_LOGISTICAL_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

struct DataFrame
{
    std::vector<std::vector<double>> feature{};
    std::vector<double> label{}; 
    std::vector<double> weight{};
    double bias{};

    size_t row() const
    {
        size_t row{this->feature[0].size()};
        return row;
    }

    size_t col() const
    {
        size_t col{this->feature.size()};
        return col;
    }
};

class LogisticalRegression
{
private:

    double m_learning_rate{0.01};
    double m_lambda{0.01};
    std::vector<double> y_hat{};
    
    //generic dot product 
    double dot(const std::vector<double> &x, const std::vector<double> &weights) const
    {
        double result {0.0};
        for(auto i{0}; i<x.size(); ++i)
        {
            result += x[i] * weights[i];
        }
        return result;
    }

    /*
    Dot product for transposed where each double vector stores features.
    Thus we need to use an index to address a dataset i
    */
    double dot(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &weights,
        const size_t &data_index
    ) const
    {
        double result {0.0};
        for(auto i{0}; i<x[data_index].size(); ++i)
        {
            result += x[i][data_index] * weights[i];
        }
        return result;
    }

    double sigmoid(const double &z) const
    {
        double y_hat {1.0/(1.0+(exp(-z)))};
        return y_hat;
    }

    double xentropyLoss(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y,
        const std::vector<double> &weights,
        const double &bias
    ) const
    {
        //Cross Entropy
        double loss {0.0};
        for(size_t i{0}; i < x[0].size(); i++)
        {
            double y_hat = predict(x,weights,bias,i);
            loss += y[i] * log(y_hat) + (1 - y[i]) * log(1 - y_hat);
        }
        loss = -loss / x[0].size();
        
        //Regularized Loss
        double weight_sum {0.0};
        for(auto& n:weights)
        {
            weight_sum += n*n;
        }
        loss += m_lambda * weight_sum;

        loss += m_lambda * bias * bias;
        return loss;
    }

    void updateWeights(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y,
        std::vector<double> &weights,
        const std::vector<double> &y_hat
    ) const
    {
        //outerloop selects feature associated weight j
        for(size_t j; j < x.size(); j++)
        {
            double dw {0.0};
            for(size_t i; i < x[0].size(); i++)
            {
                dw += (y_hat[i]-y[i]) * x[j][i];
            }
            dw /= static_cast<double>(x[0].size());
            dw += 2.0 * m_lambda * weights[j];
            weights[j] -= m_learning_rate * dw;
        }
    }

    void updateBias(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y,
        double bias,
        const std::vector<double> &y_hat
    ) const
    {
        double db {0.0};
        for(size_t i; i< y.size(); i++)
        {
            db += (y_hat[i]-y[i]);
        }
        db /= static_cast<double>(y.size());
        db += 2.0 * m_lambda * bias;
        bias -= m_learning_rate * db;
    }
    
public:
    //prediction for feature aligned vector
    double predict(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &weights,
        const double &bias,
        const size_t &data_index
    ) const
    {
        double z {dot(x,weights,data_index) + bias};
        return sigmoid(z);
    }

    //Debugging
    void printState(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y,
        std::vector<double> weights,
        double bias
    )
    {
        double loss {xentropyLoss(x,y,weights,bias)};
        std::cout << "Cross Entropy Loss: " << loss << '\n';

    }
};

#endif