#ifndef SKLEARN_LOGISTICAL_HPP
#define SKLEARN_LOGISTICAL_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm> // Required for std::max_element
#include <iterator>  // Required for std::distance

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
            double m_lambda{0.00005}; // Regularization parameter
            double correct_predictions = 0;
            double loss = 0.0;
            
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
                return y_hat = exp(z) / (ez);
            }

        public:
            //Constructor
            LogisticalRegressionMulticlass(int features, double learningrate, int classes)
            :m_learningrate{learningrate}
            {
                m_weights.resize(classes, std::vector<double>(features,0.01));
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
                std::vector<double> z = z2;
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
            //cross entropy loss
            void xentropyLoss(
                const std::vector<double> &X,
                const std::vector<double> &Y,
                const int img_count
            ) 
            {

                loss = 0.0;

                int true_class = Y[img_count];
                const std::vector<double>& feature = X;


                loss -= log(y_hat[true_class]);


                double weights_squared{0.0};
                double biases_squared{0.0};

                for (size_t k{0}; k < m_weights.size(); ++k)
                {
                    for(double w: m_weights[k])
                    {
                        weights_squared += w * w;
                    }
                    biases_squared += m_bias[k] * m_bias[k];
                }

                loss += m_lambda * (weights_squared + biases_squared);
                         
            }
            // Gradient descent method is Stochastic Gradient descent, updating after each image. This makes it reach a lower cost faster
            void gradientDescent(
                 std::vector<double> &X,
                 std::vector<double> &Y,
                 int img_count
            ) 
            {
                int true_class = Y[img_count];
                double gradient {0.0};
                std::vector<std::vector<double>> m_w = m_weights;
                std::vector<double> m_b = m_bias;
                const std::vector<double>& feature = X;


                // Where k is classes and i is feature.
                for (size_t k{0}; k < m_weights.size(); ++k)
                {
                    if (k == true_class)
                        gradient = y_hat[k] - 1;
                    else
                        gradient = y_hat[k] - 0;
                    m_b[k] -= m_learningrate * (gradient + 2 * m_lambda * m_bias[k]); 

                    for(size_t i{0}; i < m_w[k].size(); ++i)
                    {
                        m_w[k][i] -= (m_learningrate) * (gradient * feature[i] + 2*m_lambda * m_weights[k][i]); 
                    }
                }
                    //update m_weights and m_bias as m_w and m_b
                m_weights = m_w;
                m_bias = m_b;
                //return gradient;
            }
            std::vector<double> fit(
                 std::vector<std::vector<double>> &X,
                 std::vector<double> &Y,
                const int epochs
            ) 
            {
                std::vector<double> c = costs;
                std::vector<double> cost_epoch;
                for (int j = 0; j < epochs; ++j)
                {
                    double final_loss {0.0};
                    correct_predictions = 0;
                    
                    for (size_t img_id = 0; img_id < X.size(); ++img_id)  
                    {   
                        // Calculates probability of each class, calculates loss finally checks accuracy.
                        predict(X[img_id]);
                        xentropyLoss(X[img_id], Y, img_id);
                        accuracy_score(img_id, Y);
                        
                        c.push_back(loss);
                        std::vector<double>& feature{X[img_id]};
                        gradientDescent(feature, Y, img_id);
                        final_loss += loss;
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

    class DatasetChecker 
    {
        public:
            void Check_data(const std::vector<std::vector<double>> X, const std::vector<double> Y,  int Num_class)
            {
                Check_dimension( X, Y);

                for (int i = 0; i < X.size(); ++i)
                {
                    for (int j = 0; j < X[i].size(); ++j)
                    {
                        Check_nan_inf( X[i][j]);
                        Check_normalized( X[i][j]);
                    }
                }
                
                for (int i = 0; i < Y.size(); ++i )
                {
                    Check_labels(Y[i], Num_class);
                }

            }


            void Check_dimension(const std::vector<std::vector<double>> X, const std::vector<double> Y) const
            {
                if (X.empty())
                {
                    throw std::invalid_argument("The dataset is empty");
                }

                if (X.size() != Y.size())
                {
                    throw std::invalid_argument("File is not Dimensionally correct");
                }

                int start_shape = X[0].size();

                for (int i = 0 ; i < X.size(); ++i )
                {
                    if (X[i].size() != start_shape)
                    {
                        throw std::invalid_argument("File is not Dimensionally correct");
                    }
                }
            }


            void Check_nan_inf(double Value) const
            {
                if (std::isnan(Value) || std::isinf(Value) )
                {
                    throw std::invalid_argument("Nan or inf value found in file");
                }
            }


            void Check_normalized(double Value) const
            {
                if (Value < 0 || Value > 1)
                {
                    throw std::invalid_argument("Values are not properly normalized");
                }
            }


            void Check_labels(double Value, int N_class) const
            {
                Check_nan_inf(Value);

                if (Value < 0 || Value >= N_class || std::fmod(Value, 1.0) != 0.0)
                {
                    throw std::invalid_argument("Labels are anomalous");
                }
            }

    };
}



#endif