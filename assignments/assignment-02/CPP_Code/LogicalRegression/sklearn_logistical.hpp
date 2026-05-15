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

            std::vector<std::vector<double>> class_weights;
            std::vector<double> class_bias;
            std::vector<double> probabilities;
            double learningrate{0.0};
            double lambda{0.00005}; // Regularization parameter
            double correct_predictions = 0;
            double loss = 0.0;
            
            /*
            This function checks the highest predicted probability agianst the true class,
            if correct it updates correct prediction.
            */
            void accuracy_score(
                const int image_count,
                const std::vector<double>& Y
            )
            {
                auto max_it = std::max_element(probabilities.begin(), probabilities.end());
                double max_value = *max_it;
                int position = std::distance(probabilities.begin(), max_it);
                if (position == (Y[image_count]))
                {
                    ++correct_predictions;
                }
            }

            double dot(const std::vector<double> &X, const int class_idx) const
            {
                // dot product of x.w
                double result {0.0};
                // size_t cause .size() returns maximum size of vector.
                for(size_t i{0}; i < X.size(); ++i)
                {
                    result += X[i] * class_weights[class_idx][i];
                }
                return result;
            }
            /*
            This function performs softmax, producing probabilities of
            each class being the designated photos true value.
            e.g {0.05, 0.1, 0.000013, 0.1, 0.3, 0.1, 0.125, 0.05, 0.1, 0.025,}
            where each index is class 0-9. The one above is predicting
            class 5 to be the image.
            */
            void compute_probabilities(
                const std::vector<double> &x
            ) 
            {   
                probabilities.clear();
                double sum_linear_exp {0.0};
                double probability {0.0};
                std::vector<double> linear_combination;
                std::vector<double> temp;
                // linear equation 
                for (size_t i{0}; i < class_bias.size(); ++i)
                {
                    linear_combination.push_back(dot(x, i) + class_bias[i]);
                }
                // summing to make; Sum e^{z_k} where k = 1, 2, 3 .... K
                for (size_t i{0}; i < linear_combination.size(); ++i)
                {
                    sum_linear_exp += exp(linear_combination[i]);
                }
                // perform softmax probability on each class.
                for (size_t i{0}; i < class_bias.size(); ++i)
                {
                    temp.push_back(exp(linear_combination[i]) / (sum_linear_exp));
                }
                
                probabilities = temp;
            }

            /*
            Cross entropy loss function. This calculates the loss function for each
            image. Checking how far away it is from the true class. This includes regularization.
            */
            void xentropyLoss(
                const std::vector<double> &X,
                const std::vector<double> &Y,
                const int img_count
            ) 
            {

                loss = 0.0;

                int true_class = Y[img_count];
                loss -= log(probabilities[true_class]);


                double weights_squared{0.0};
                double biases_squared{0.0};

                for (size_t i{0}; i < class_weights.size(); ++i)
                {
                    for(double temp_w: class_weights[i])
                    {
                        weights_squared += temp_w * temp_w;
                    }
                    biases_squared += class_bias[i] * class_bias[i];
                }

                loss += lambda * (weights_squared + biases_squared);
                         
            }

            /* 
            Gradient descent method is Stochastic Gradient descent, updating after each image.
            This makes it reach a lower much faster, giving the ability to use much lower epoch amounts.
            With current parameters making it reach convergence at ~29 epochs, converging at ~92% accuracy, 
            which from research indicates to be the theoritcal limit for non convolution neural networks.

            The other option is using batch training, which while being slower at training is less demanding of the 
            pc and not being as slow.
            */
            void gradientDescent(
                 std::vector<double> &X,
                 std::vector<double> &Y,
                 int img_count
            ) 
            {
                int true_class = Y[img_count];
                double gradient {0.0};
                const std::vector<double>& feature = X;

                // Where k is classes and i is feature.
                for (size_t k{0}; k < class_weights.size(); ++k)
                {
                    if (k == true_class)
                        gradient = probabilities[k] - 1;
                    else
                        gradient = probabilities[k] - 0;
                    class_bias[k] -= learningrate * (gradient + 2 * lambda * class_bias[k]); 

                    for(size_t i{0}; i < class_weights[k].size(); ++i)
                    {
                        class_weights[k][i] -= (learningrate) * (gradient * feature[i] + 2*lambda * class_weights[k][i]); 
                    }
                }
            }

        public:
            // Constructor
            LogisticalRegressionMulticlass(int features, double temp_learningrate, int classes)
            :learningrate{temp_learningrate}
            {
                class_weights.resize(classes, std::vector<double>(features, 0.01));
                class_bias.resize(classes, 0.0);
            }

            /*
            This fit function trains the data and updates the weights and biases through running the 
            gradient descent function. As well as keeping track of costs and showing training stats of accuracy
            and cost.
            */
            std::vector<double> fit(
                 std::vector<std::vector<double>> &X,
                 std::vector<double> &Y,
                const int epochs
            ) 
            {
                std::vector<double> cost_epoch;
                for (int epoch = 0; epoch < epochs; ++epoch)
                {
                    double final_loss {0.0};
                    correct_predictions = 0;
                    
                    for (size_t img_id = 0; img_id < X.size(); ++img_id)  
                    {   
                        // calculates probability of each class, calculates loss finally checks accuracy.
                        compute_probabilities(X[img_id]);
                        xentropyLoss(X[img_id], Y, img_id);
                        accuracy_score(img_id, Y);
                        
                        std::vector<double>& feature{X[img_id]};
                        gradientDescent(feature, Y, img_id);
                        final_loss += loss;
                    }
                    std::cout << "cost at epoch " << epoch + 1 << ": " << final_loss / X.size() << " Accuracy: " << correct_predictions << "/" << X.size() << " percentage: " << (double)correct_predictions / X.size() * 100 << "% " << std::endl;
                    cost_epoch.push_back(final_loss / X.size());
                
                    /*
                     Input a converging check based on the cost function,
                     if the cost function is not decreasing by a certain amount then declare convergence.
                     This would essentially mean that it has reached a local minimum in the gradient descent.
                    */
                    int cost_epoch_len = cost_epoch.size();
                    if (cost_epoch_len >= 3 )
                    {
                        double max_recent_cost_epoch = std::max({cost_epoch[cost_epoch_len-1], cost_epoch[cost_epoch_len-2], cost_epoch[cost_epoch_len-3]});
                        double min_recent_cost_epoch = std::min({cost_epoch[cost_epoch_len-1], cost_epoch[cost_epoch_len-2], cost_epoch[cost_epoch_len-3]});
                        double threshold_range_cost_epoch = max_recent_cost_epoch - min_recent_cost_epoch;

                        if ( threshold_range_cost_epoch <= 0.001 )
                        {
                            std::cout << "Convergence : Loss function settled" << std::endl;
                            break;
                        }
                    }
                }
                return cost_epoch;
            }
            /*
            This function performs a prediction based on the weights it has
            aggregated over the training.
            */
            void predict(
                const std::vector<std::vector<double>>& x,
                const std::vector<double>& y)
            { 
                correct_predictions = 0;
                for (size_t img_id = 0; img_id < x.size(); ++img_id)  
                {   
                    compute_probabilities(x[img_id]);          
                    accuracy_score(img_id, y);      
                }
                std::cout << " Accuracy: " << correct_predictions << "/" << x.size() << " percentage: " << (double)correct_predictions / x.size() * 100 << "% " << std::endl;     
            }   
    };
    // This is preprocessing for the data thats being input. Checking various errors.
    class dataset_checker 
    {
        public:
            // Checking each value in data for error.
            void check_data(const std::vector<std::vector<double>> X, const std::vector<double> Y,  int num_class)
            {
                check_dimension( X, Y);

                for (int i = 0; i < X.size(); ++i)
                {
                    for (int j = 0; j < X[i].size(); ++j)
                    {
                        check_nan_inf( X[i][j]);
                        check_normalized( X[i][j]);
                        
                    }
                }
            }

            // Checks that dimensions match up that all features are the same.
            void check_dimension(const std::vector<std::vector<double>> X, const std::vector<double> Y) const
            {
                if (X.empty())
                {
                    throw std::invalid_argument("The dataset is empty");
                }

                if (X.size() != Y.size())
                {
                    throw std::invalid_argument("File is not dimensionally correct");
                }

                int start_shape = X[0].size();

                for (int i = 0 ; i < X.size(); ++i )
                {
                    if (X[i].size() != start_shape)
                    {
                        throw std::invalid_argument("File is not dimensionally correct");
                    }
                }
            }

            // Checks for nan's and inf's in data.
            void check_nan_inf(double value) const
            {
                if (std::isnan(value) || std::isinf(value) )
                {
                    throw std::invalid_argument("NaN or inf value found in file");
                }
            }

            // Check if data is within boundary for model.
            void check_normalized(double value) const
            {
                if (value < 0 || value > 1)
                {
                    throw std::invalid_argument("Values are not properly normalized");
                }
            }
    };
}

#endif