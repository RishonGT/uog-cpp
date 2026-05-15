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

    void resize(size_t i)
    {
        label.resize(i);
        for (auto& dataset : feature)
        {
            dataset.resize(i);
        }
    }

    void print() const
    {
    std::cout << "Datasets: "<< row() <<" "<< "Features: "<< col() << '\n';
    }
};

class LogisticalRegression
{
private:

    double m_learning_rate{0.001};
    double m_lambda{0.001};
    std::vector<double> m_weight{};
    double m_bias{0.01};
    int m_max_iteration{10000};
    double m_tolerance{0.0001};
    
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
        for(auto i{0}; i<weights.size(); ++i)
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
        const std::vector<double> &y
    ) const
    {
        //Cross Entropy
        double loss {0.0};
        double y_hat{};
        for(size_t i{0}; i < y.size(); i++)
        {
            y_hat = predict(x,m_weight,m_bias,i);
            loss += y[i] * log(y_hat+1e-9);
            loss += (1.0 - y[i]) * (log(1.0-y_hat+1e-9));

        }
        loss = (-1.0 * loss) / static_cast<double>(y.size());
        
        //Regularized Loss
        double weight_sum {0.0};
        for(auto& n:m_weight)
        {
            weight_sum += n*n;
        }
        loss += m_lambda * weight_sum;
        loss += m_lambda * m_bias * m_bias;
        return loss;
    }

    void updateWeights(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y,
        const std::vector<double> &y_hat
    ) 
    {
        //outerloop selects feature associated weight j
        double dw {0.0};
        for(size_t j{0}; j < x.size(); j++)
        {
            dw = 0.0;
            for(size_t i{0}; i < x[0].size(); i++)
            {
                dw += (y_hat[i]-y[i]) * x[j][i];
            }
            dw /= static_cast<double>(x[0].size());
            dw += 2.0 * m_lambda * m_weight[j];
            m_weight[j] -= m_learning_rate * dw;
        }
    }

    void updateBias(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y,
        const std::vector<double> &y_hat
    ) 
    {
        double db {0.0};
        for(size_t i{0}; i< y.size(); i++)
        {
            db += (y_hat[i]-y[i]);
        }
        db /= static_cast<double>(y.size());
        db += 2.0 * m_lambda * m_bias;
        m_bias -= m_learning_rate * db;
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

    double predict(
        const std::vector<std::vector<double>> &x,
        const size_t &data_index
    )
    {
        double z {dot(x,m_weight,data_index) + m_bias};
        return sigmoid(z);
    }

    void train(
        const std::vector<std::vector<double>> &x,
        const std::vector<double> &y
    )
    {
        m_weight.resize(x.size()); //initialize weights to match features
        std::vector<double> y_hat(y.size());
        double previous_loss{INFINITY};
        for(size_t iteration{0}; iteration <= m_max_iteration; iteration++)
        {
            /*Prediction
            We use y_hat twice in weights and bias updates. Probably better to do it once and store the values
            Attempting to predict in place drastically increases time per epoch, probably vector<vector<double>> is not
            a flat vector*/
            for(size_t i{0}; i < y.size(); i++)
            {
                y_hat[i]=predict(x,m_weight,m_bias,i);
            }

            updateWeights(x,y,y_hat);
            updateBias(x,y,y_hat);

            //Record Cost and Decision if tolerances are met.
            double current_loss {xentropyLoss(x,y)};
            if(iteration % 100 == 0){std::cout <<"Epoch :" << iteration << " Cost: " << current_loss <<'\n';}

            if(std::abs(previous_loss - current_loss) < m_tolerance)
            {
                std::cout << "Convergence reached at iteration: " << iteration << ", Loss: " << current_loss << '\n';
                break;
            }
            previous_loss = current_loss;
        }
        std::cout << "Training Complete" << '\n';
    }
};

#endif