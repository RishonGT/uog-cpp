#ifndef SKLEARN_LOGISTICAL_HPP
#define SKLEARN_LOGISTICAL_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm> 
#include <iterator>  

namespace linear_model
{
    class LogisticalRegressionMulticlass
    {
        private:

            std::vector<std::vector<double>> m_weights;
            std::vector<double> costs;
            std::vector<double> m_bias;
            std::vector<double> z2;
            std::vector<double> y_hat;
            double m_learningrate{0.0};
            double m_lambda{0.0000005}; // Regularization parameter
            double correct_predictions = 0;
            
            void accuracy_score(
                const int image_count,
                const std::vector<double>& Y
            )
            {
                auto max_it = std::max_element(y_hat.begin(), y_hat.end());
                double max_value = *max_it;
                int position = std::distance(y_hat.begin(), max_it);
                if (position == (Y[image_count]))
                {
                    ++correct_predictions;
                }
            }
            
            
            double softmax(const double &z) const
            //prediction of y_hat
            {
                double ez {0.0};
                double y_hat {0.0};
                for (size_t i{0}; i < z2.size(); ++i)
                    {
                        ez += exp(z2[i]);
                    }
                return y_hat = exp(z) / (ez); // Add a small constant to prevent division by zero
            }

        public:
            //Constructor
            LogisticalRegressionMulticlass(int features, double learningrate, int classes)
            :m_learningrate{learningrate}
            {
                m_weights.resize(classes, std::vector<double>(features, 0.1));
                m_bias.resize(classes, 0.0);
            }

            // Functions
            double dot(const std::vector<double> &X, const int class_idx) const
            {
                //dot product of x.w
                double result {0.0};
                //size_t cause .size() returns whatever that is
                for(size_t i{0}; i<X.size(); ++i)
                    {
                        result += X[i] * m_weights[class_idx][i];
                    }

                return result;
            }

            void predict(
                const std::vector<double> &x
            ) 
            {   
                z2.clear();
                y_hat.clear();
                std::vector<double> z = z2 ;
                std::vector<double> y_h = y_hat;
                for (size_t i{0}; i < m_bias.size(); ++i)
                {
                    double lin = dot(x, i) + m_bias[i];
                    z.push_back(lin);
                    if (i == m_bias.size() - 1)
                    {
                        z2 = z;
                        for (size_t j{0}; j < m_bias.size(); ++j)
                        {
                            y_h.push_back(softmax(z[j]));
                        }
                    }
                
                }
                y_hat = y_h;
            }

            //cross entropy loss
            double xentropyLoss(
                const std::vector<double> &X,
                const std::vector<double> &Y,
                const int img_count
            ) 
            {
            double loss {0.0};
            loss = -log(y_hat[(Y[img_count])]);

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

            void gradientDescent(
                 std::vector<double> &X,
                 std::vector<double> &Y,
                 int img_count
            ) 
            {
                double gradient {0.0};
                std::vector<std::vector<double>> m_w = m_weights;
                std::vector<double> m_b = m_bias;
                int exp_classidx = Y[img_count];
                const std::vector<double>& feature = X;

                
                for (size_t k{0}; k < m_weights.size(); ++k)
                    {
                        if (k == exp_classidx)
                            gradient = y_hat[k] - 1;
                        else
                            gradient = y_hat[k] - 0;
                        
                        m_b[k] -= (m_learningrate * gradient * (1+2*m_lambda * m_bias[k])); 

                        for(size_t i{0}; i < m_w[k].size(); ++i)
                            {
                                m_w[k][i] -= (m_learningrate * gradient * feature[i] + 2*m_lambda * m_weights[k][i]); 
                            }
                    }
                    //update m_weights and m_bias as m_w and m_b
                    m_weights = m_w;
                    m_bias = m_b;
                //return gradient;
            }
            std::vector<double> train(
                 std::vector<std::vector<double>> &X,
                 std::vector<double> &Y,
                const int epochs
            ) 
            {
                std::vector<double> cost_epoch;
                for (int j = 0; j < epochs; ++j)
                        {
                            double final_loss {0.0};
                            correct_predictions = 0;
                            std::vector<double> c = costs;
                            for (size_t img_id = 0; img_id < X.size(); ++img_id)  
                                {   
                                    predict(X[img_id]);
                                    //cal cost
                                    const double je = xentropyLoss(X[img_id], Y, img_id);
                                    accuracy_score(img_id, Y);
                                    std::vector<double> z = z2;
                                    /*
                                    if (img_id % 200 == 0)
                                    {
                                        std::cout << "z at img " << img_id << " true value " << Y[img_id] << "\n";
                                        for (size_t i{0}; i < y_hat.size(); ++i)
                                        {
                                            std::cout << "y_hat at img " << img_id << " for class " << i << ": " << y_hat[i] << std::endl;
                                        }
                                    }
                                        */
                                    //std::cout << "cost at img" << y_hat[0] << ": " << je << std::endl;
                                    c.push_back(je);
                                    std::vector<double>& feature{X[img_id]};
                                    gradientDescent(feature, Y, img_id);
                                    final_loss += je;
                                }
                            std::cout << "Cost at epoch " << j + 1 << ": " << final_loss / X.size() << " Accuracy: " << correct_predictions << "/" << X.size() << " percentage: " << (double)correct_predictions / X.size() * 100 << "% " << std::endl;
                            //cost_epoch.push_back(final_loss / X.size());
                            
                            // imput an early stopping condition here based on the cost function, if the cost function is not decreasing by a certain amount then stop training.
                        }
                return cost_epoch;
            }
            
            void predict_picture(
                const std::vector<std::vector<double>>& x,
                const std::vector<double>& y)
            {
                correct_predictions = 0;
                for (size_t img_id = 0; img_id < x.size(); ++img_id)  
                    {   
                        predict(x[img_id]);          
                        accuracy_score(img_id, y);      
                    }
                std::cout << " Accuracy: " << correct_predictions << "/" << x.size() << " percentage: " << (double)correct_predictions / x.size() * 100 << "% " << std::endl;     
            }
                
    };
};



#endif