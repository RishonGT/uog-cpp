#ifndef SKLEARN_CPP_HPP
#define SKLEARN_CPP_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>
#include <set>
#include <memory>
#include <algorithm> 
#include <iterator>  



namespace sklearn_cpp{
    namespace linear_model {
        class LinearRegression {
            private:
            // Model parameters
                std::vector<double> weights;  // Coefficients for each feature
                double intercept;  // Intercept term
                bool fitted;  // Flag to indicate if the model has been fitted

                // Function to calculate R² score
                static double r2_score(
                    const std::vector<std::vector<double>>& x,
                    const std::vector<double>& y,
                    const std::vector<double>& w,
                    const double& b
                ){
                    const int m {static_cast<int>(y.size())};

                    double ss_res  {0.0}; // Residual sum of squares
                    double ss_tot  {0.0}; // Total sum of squares
                    double mean_y  {0.0};

                    mean_y = mean_func(y);

                    // Calculate predictions and sums for R² score
                    for (int i=0; i < m; i++) {
                        double prediction {b};
                        for (std::size_t feature = 0; feature < w.size(); ++feature) {
                            prediction += w[feature] * x[feature][i];
                        }
                        ss_res += (y[i] - prediction) * (y[i] - prediction);
                        ss_tot += (y[i] - mean_y) * (y[i] - mean_y);
                    }

                    if (ss_tot == 0.0) {
                        return (ss_res == 0.0) ? 1.0 : 0.0;
                    }

                    return 1 - (ss_res / ss_tot);

                }


                // Function to calculate mean squared error loss
                static double loss_function(
                    const std::vector<std::vector<double>>& x,
                    const std::vector<double>& y,
                    const std::vector<double>& w,
                    const double& b
                ) {
                    const int m {static_cast<int>(y.size())};
                    double total_loss {0.0};

                    // Calculate predictions and total loss
                    for (int i=0; i < m; i++) {
                        double prediction  {b};
                        for (std::size_t feature = 0; feature < w.size(); ++feature) {
                            prediction += w[feature] * x[feature][i];
                        }
                        total_loss += (y[i] - prediction) * (y[i] - prediction);  // Squared error for the current sample
                    }
                    // Return mean squared error
                    return total_loss / m;
                }
                
                // Helper functions for normalization
                // Function to calculate mean of a vector
                static double mean_func(const std::vector<double>& data) {
                    double sum {0.0};
                    for (double value : data) {
                        sum += value;
                    }
                    return sum / data.size();
                }

                // Function to calculate standard deviation of a vector
                static double std_dev_func(const std::vector<double>& data, const double& mean) {
                    double std_dev {0.0};
                    for (double value : data) {
                        std_dev += (value - mean) * (value - mean);
                    }
                    return sqrt(std_dev / data.size());
                }

                // Function to normalize a vector using mean and standard deviation
                static std::vector<double> normalise(
                    const std::vector<double>& data,
                    double& mean,
                    double& std_dev
                ) {
                    mean = mean_func(data);
                    std_dev = std_dev_func(data, mean);

                    std::vector<double> normalised_data(data.size());

                    // Handle the case where standard deviation is zero to avoid division by zero
                    if (std_dev == 0.0) {
                        return normalised_data;
                    }

                    
                    for (std::size_t i = 0; i < data.size(); ++i) {
                        normalised_data[i] = (data[i] - mean) / std_dev;
                    }
                    return normalised_data;

                }


            public:
                LinearRegression() : intercept(0.0), fitted(false) {}
                
                // Function to fit the linear regression model to the data
                double score(
                    const std::vector<std::vector<double>>& X, 
                    const std::vector<double>& Y
                ) const{
                    // Validate input and model state before calculating R² score
                    // Check if the model is fitted
                    if (!fitted) {
                        throw std::logic_error("score: model is not fitted yet");
                    }

                    // Validate input data dimensions
                    if (X.empty() || Y.empty() || X[0].size() != Y.size()) {
                        throw std::invalid_argument("score: input data is invalid. Ensure X and Y are non-empty and have compatible dimensions.");
                    }
                    if (weights.empty()) {
                        throw std::logic_error("score: model has no weights. Ensure the model is fitted properly.");
                    }
                    if (X.size() != weights.size()) {
                        throw std::invalid_argument("score: number of features in X must match the number of weights");
                    }

                    return r2_score(X, Y, weights, intercept);
                }
                
                // Function to fit the linear regression model to the data
                bool fit(
                    std::vector<std::vector<double>>& X, 
                    std::vector<double>& Y
                ) {
                    // Validate input data before fitting the model
                    fitted = false;
                    weights.clear();
                    intercept = 0.0;

                    // Check if X and Y are non-empty and have compatible dimensions
                    if (X.empty() || Y.empty() || X[0].size() != Y.size()) {
                        std::cerr << "Invalid input data. Ensure X and Y are non-empty and have compatible dimensions." << std::endl;
                        return false;
                    }

                    // Check if there are at least two data points to fit the model
                    if (Y.size() < 2) {
                        std::cerr << "At least two data points are required to fit the model." << std::endl;
                        return false;
                    }

                    double Y_std {std_dev_func(Y, mean_func(Y))};
                    // Check for zero variance in the target variable
                    if (Y_std == 0.0) {
                        std::cerr << "All target values are the same. The model cannot be fitted." << std::endl;
                        return false;
                    }

                    // Check for zero variance in each feature
                    for (const std::vector<double>& feature_column : X) {
                        double feature_std {std_dev_func(feature_column, mean_func(feature_column))};
                        if (feature_std == 0.0) {
                            std::cerr << "One of the features has zero variance. The model cannot be fitted." << std::endl;
                            return false;
                        }
                    }

                    // Validate that each feature column in X has the same number of samples as Y
                    const int variables {static_cast<int>(X.size())};
                    const int m {static_cast<int>(Y.size())}; // Number of data points

                    for (int feature = 0; feature < variables; ++feature) {
                        if (static_cast<int>(X[feature].size()) != m) {
                            throw std::invalid_argument("fit: each feature column in X must match Y length");
                        }
                    }


                    weights.assign(variables, 0.0); // Initialize weights to zero
                    double b {0.0}; // Initialize intercept to zero

                    // Normalize features and target variable
                    std::vector<double> means(variables), std_devs(variables);
                    std::vector<std::vector<double>> x_normalised(variables);

                    // Normalize each feature column in X
                    for (int feature = 0; feature < variables; feature++) {
                        x_normalised[feature] = normalise(X[feature], means[feature], std_devs[feature]);
                        if (std_devs[feature] == 0.0) {
                            std::cerr << "Feature " << feature << " has zero variance after normalization. The model cannot be fitted." << std::endl;
                            return false;
                        }
                    }

                    // Normalize target variable Y
                    double mean_y {0.0}, std_dev_y {1.0};

                    std::vector<double> y_normalised {normalise(Y, mean_y, std_dev_y)};

                    // Check for zero variance in the normalized target variable
                    if (std_dev_y == 0.0) {
                        std::cerr << "Target variable has zero variance after normalization. The model cannot be fitted." << std::endl;
                        return false;
                    }

                    // Gradient descent to optimize weights and intercept
                    std::vector<double> w {weights};
                    double learning_rate {0.01};
                    int max_iterations {10000};
                    int iteration {0};
                    const double tolerance {1e-13};

                    // Calculate initial loss before starting gradient descent
                    double prev_loss {loss_function(x_normalised, y_normalised, w, b)};

                    // Main loop for gradient descent optimization
                    for (iteration = 0; iteration < max_iterations; iteration++) {
                        std::vector<double> dw(w.size(), 0.0); // Gradient for w
                        double db {0.0}; // Gradient for b

                        // Calculate gradients
                        for (int sample = 0; sample < m; sample++) {
                            double pred {b};
                            for (int feature = 0; feature < variables; feature++) {
                                pred += w[feature] * x_normalised[feature][sample];
                            }
                            
                            double residual {pred - y_normalised[sample]}; // Residual for the current sample
                            for (int feature = 0; feature < variables; feature++) {
                                dw[feature] += residual * x_normalised[feature][sample]; // Derivative with respect to w[feature]
                            }
                            db += residual; // Derivative with respect to b
                    
                        }

                        for (int feature = 0; feature < variables; feature++) {
                            dw[feature] /= m;
                            w[feature] -= learning_rate * dw[feature]; // Update w[feature]
                        }
                        db /= m;
                        b -= learning_rate * db; // Update b

                        // Calculate current loss after the update
                        double current_loss = loss_function(x_normalised, y_normalised, w, b);

                        if (iteration % 500 == 0) { // Print loss every 500 iterations
                            std::cout << "Iteration: " << iteration << ", Loss: " << current_loss << std::endl;
                        }

                        // Check for convergence based on loss improvement and maximum iterations
                        if (iteration >= max_iterations) {
                            break;
                        }

                        if (std::abs(prev_loss - current_loss) < tolerance) {
                            std::cout << "Convergence reached at iteration: " << iteration << ", Loss: " << current_loss << std::endl;
                            break;
                        }
                        prev_loss = current_loss;
                    }
                    
                    // Denormalize coefficients:
                    // w_orig[j] = w_norm[j] * (y_std / x_std[j])
                    // b_orig = y_mean + y_std * b_norm - sum(w_orig[j] * x_mean[j])
                    std::vector<double> w_denorm(variables, 0.0);
                    for (int feature = 0; feature < variables; ++feature) {
                        w_denorm[feature] = w[feature] * (std_dev_y / std_devs[feature]);
                    }

                    double b_denorm = mean_y + std_dev_y * b;
                    for (int feature = 0; feature < variables; ++feature) {
                        b_denorm -= w_denorm[feature] * means[feature];
                    }

                    weights = w_denorm;
                    intercept = b_denorm;
                    fitted = true;

                    // Print the final equation of the fitted model
                    std::cout << "Training completed. Final equation: \n\nY = ";
                    for (std::size_t i = 0; i < weights.size(); ++i) {
                        std::cout << weights[i] << " * X" << i + 1;
                        if (i < weights.size() - 1) {
                            std::cout << " + ";
                        }
                    }            
                    // Print the intercept term
                    std::cout << " + " << intercept << "\n" << std::endl;
                    

                    return true;

                };

                // Function to make predictions using the fitted model
                double predict(const std::vector<double>& x) const {
                    // Validate model state and input before making predictions
                    if (!fitted) {
                        throw std::logic_error("predict: model is not fitted yet");
                    }
                    
                    // Check if input feature vector size matches the number of weights
                    if (x.size() != weights.size()) {
                        throw std::invalid_argument("predict: input feature vector size must match the number of weights");
                    }

                    double result {0.0};
                    for (std::size_t i = 0; i < weights.size(); ++i) {
                        result += weights[i] * x[i];
                    }
                    return result + intercept;
                }
            };
        
        
        /* ILogistical class is the parent virtual class, it ensurse two things that fit and predict functions are avaliable
        fromt the child classes. This is needed because the pointer can not be pointed towards either a binary or multiclass,
        it would mean it only works as binary or multiclass.

        This interface being a parent, allows us to point to both because both classes are a child ILogisticalInterface

        The actual path and correct child is selected in the LogisticRegression class all the way at the bottom.
        Classes must ALSO be declared in order, so the class handler must be the last thing that is created.
        */
        class ILogisticalInterface {
            public:
                //
                virtual ~ILogisticalInterface() = default;

                //Virtual functions for multiclassification
                virtual std::vector<double> fit(
                    std::vector<std::vector<double>> &X,
                    std::vector<double> &Y) = 0;

                virtual void predict(
                    const std::vector<std::vector<double>> &x,
                    const std::vector<double> &y) = 0;

                //Virutal function for binary
                virtual void fit(
                    const std::vector<std::vector<double>> &x,
                    const std::vector<double> &y) = 0;

                virtual double predict(
                    const std::vector<std::vector<double>> &x,
                    const size_t &data_index)const = 0;
                

        };
        class LogisticalRegressionBinary: public ILogisticalInterface
        {
        private:

            double m_learning_rate{0.001};
            double m_lambda{0.001};
            std::vector<double> m_weight{};
            double m_bias{0.01};
            int m_max_iteration{10000};
            double m_tolerance{0.0001};

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
                    y_hat = predict(x,i);
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

            double predict(
                const std::vector<std::vector<double>> &x,
                const size_t &data_index
            ) const override
            {
                double z {dot(x,m_weight,data_index) + m_bias};
                return sigmoid(z);
            }

            void fit(
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
                        y_hat[i]=predict(x,i);
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

            //Requires functions because we're using Pure Virtual Functions
            std::vector<double> fit(
                std::vector<std::vector<double>> &X,
                std::vector<double> &Y)override{
                    throw std::logic_error("Called the wrong function. Incorrect model used.");
                }

            void predict(
                const std::vector<std::vector<double>> &x,
                const std::vector<double> &y)override{
                    throw std::logic_error("Called the wrong function. Incorrect model used.");
                }
        };
    
        class LogisticalRegressionMulticlass: public ILogisticalInterface
        {
            private:

                std::vector<std::vector<double>> class_weights;
                std::vector<double> class_bias;
                std::vector<double> probabilities;
                double learningrate{0.0};
                double lambda{0.00005}; // Regularization parameter
                double correct_predictions = 0;
                double loss = 0.0;
                int epochs;
                
                /*
                This function checks the highest predicted probability agianst the true class,
                if correct it updates correct prediction. I could have added this to be existing in 
                fit and predict seperately, it is done like this to prevent repetition of code.
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
                    std::vector<double> linear_combination;
                    std::vector<double> temp;
                    // linear equation 
                    for (size_t i{0}; i < class_bias.size(); ++i)
                    {
                        linear_combination.push_back(dot(x, i) + class_bias[i]);
                    }
                    // Stabilize softmax by shifting with max logit to avoid overflow.
                    const double max_logit = *std::max_element(
                        linear_combination.begin(),
                        linear_combination.end()
                    );
                    // summing to make; Sum e^{z_k} where k = 1, 2, 3 .... K
                    for (size_t i{0}; i < linear_combination.size(); ++i)
                    {
                        sum_linear_exp += exp(linear_combination[i] - max_logit);
                    }
                    // perform softmax probability on each class.
                    for (size_t i{0}; i < class_bias.size(); ++i)
                    {
                        temp.push_back(exp(linear_combination[i] - max_logit) / (sum_linear_exp));
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
                 Transpose function for matrix, used as the data is stored as features major,
                 but we need to access it as sample major for the dot product.
                */
                void transpose(std::vector<std::vector<double>>& matrix) {
                    if (matrix.empty() || matrix[0].empty()) {
                        return; // Handle empty matrix case
                    }
                    const size_t rows = matrix.size();
                    const size_t cols = matrix[0].size();
                    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
                    for (size_t i = 0; i < rows; ++i) {
                        for (size_t j = 0; j < cols; ++j) {
                            transposed[j][i] = matrix[i][j];
                        }
                    }
                    matrix = std::move(transposed);
                }

                /* 
                Gradient descent method is Stochastic Gradient descent, updating after each image.
                This makes it reach a lower much faster, giving the ability to use much lower epoch amounts.

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
                LogisticalRegressionMulticlass(int features, double temp_learningrate, int classes, int temp_epochs )
                :
                learningrate{temp_learningrate},
                epochs{temp_epochs}
                
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
                    std::vector<double> &Y
                ) override
                {
                    transpose(X); // Transpose the feature matrix to access samples as rows
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
                    const std::vector<double>& y) override
                { 
                    std::vector<std::vector<double>> x_transposed = x;
                    
                    transpose(x_transposed); // Transpose the feature matrix to access samples as rows
                    correct_predictions = 0;
                    for (size_t img_id = 0; img_id < x_transposed.size(); ++img_id)  
                    {   
                        compute_probabilities(x_transposed[img_id]);          
                        accuracy_score(img_id, y);      
                    }
                    std::cout << " Accuracy: " << correct_predictions << "/" << x_transposed.size() << " percentage: " << (double)correct_predictions / x_transposed.size() * 100 << "% " << std::endl;     
                }
                
                //Functions required for Virtual Pure
                void fit(
                    const std::vector<std::vector<double>> &x,
                    const std::vector<double> &y
                ) override{
                    throw std::logic_error("Incorrect Fit Function used");
                }
                    
                double predict(
                    const std::vector<std::vector<double>> &x,
                    const size_t &data_index
                )const override{
                    throw std::logic_error("Incorrect Predict Function used");
                }

                



        };
        /*This class calls the correct binary or multiclassification class depending on label sizes.
        There are two stages, the public facing side of the class LogisticalRegression and ILogisticalInterface
        We do not know what the user wants apart from something in the LogisticalRegression library, we provide
        a user forward facing class which they interact with.

        LogisticalRegression, main purpose is to initially create a nullpointer to a potential
        LogisticalRegression by using an Interface (ILogisticalInterface).

        Using the fit() function will resolve which is the correct LogsticalRegression to use
        */   
        class LogisticRegression {
            private:
                //unique_ptr is how this magic works
                std::unique_ptr<ILogisticalInterface> impl;

                //Any variables that get passed into LogisticRegression must be declared here to be stored.
                int features{0};
                int classes{0};
                double learningrate{0.001};
                int epochs{50};
                
                // Transpose function for matrix, used as the data is stored as features major, but we need to access it as sample major for the dot product.
                void transpose(std::vector<std::vector<double>>& matrix) {
                    if (matrix.empty() || matrix[0].empty()) {
                        return; // Handle empty matrix case
                    }
                    const size_t rows = matrix.size();
                    const size_t cols = matrix[0].size();
                    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
                    for (size_t i = 0; i < rows; ++i) {
                        for (size_t j = 0; j < cols; ++j) {
                            transposed[j][i] = matrix[i][j];
                        }
                    }
                    matrix = std::move(transposed);
                }


                //This function selects the correct type to use by the difference of x and y vectors and making assumptions
                size_t selectLogisticalType(
                    const std::vector<std::vector<double>>& x,
                    const std::vector<double>& y
                ) {
                    const size_t unique_labels{std::set<double>(y.begin(), y.end()).size()};
                    std::vector<std::vector<double>> x_transposed = x;
                    transpose(x_transposed); // Transpose the feature matrix to access samples as rows
                    if (!impl) {
                        if (unique_labels == 2) {
                            impl = std::make_unique<LogisticalRegressionBinary>();
                        } else if (unique_labels > 2) {
                            // Infer features and classes if not provided, otherwise use provided values
                            const int inferred_features = features > 0
                                ? features
                                : (x_transposed.empty() ? 0 : static_cast<int>(x_transposed[0].size()));
                            const int inferred_classes = classes > 0
                                ? classes
                                : static_cast<int>(unique_labels);
                            if (inferred_features <= 0) {
                                throw std::invalid_argument("Invalid feature count for multiclass");
                            }
                            impl = std::make_unique<LogisticalRegressionMulticlass>(
                                inferred_features,
                                learningrate,
                                inferred_classes,
                                epochs
                            );
                        } else {
                            throw std::invalid_argument("Insufficent Y labels");
                        }
                    }
                    return unique_labels;
                }

            public:
            //Functions that are called from main go through here initially and tested to see which child class to use.
                LogisticRegression() = default;

                LogisticRegression(int features, double temp_learningrate, int classes, int temp_epochs)
                :
                features{features},
                classes{classes},
                learningrate{temp_learningrate},
                epochs{temp_epochs}
                {
                }
                //In this case, we use the fit function as the discriminate. Binary and Multiclassification different amount of labels.
                void fit(
                    std::vector<std::vector<double>> &x,
                    std::vector<double> &y){
                        const size_t unique_labels = selectLogisticalType(x, y);
                        if (unique_labels == 2) {
                            const auto& x_const = x;
                            const auto& y_const = y;
                            impl->fit(x_const, y_const);
                        } else {
                            (void)impl->fit(x, y);
                        }
                    }

                double predict(
                    const std::vector<std::vector<double>> &x,
                    const size_t &data_index){
                        if(!impl){
                            throw std::logic_error("predict: model is not fitted yet");
                        }
                        return impl->predict(x,data_index);
                    }

                void predict(
                    const std::vector<std::vector<double>> &x,
                    const std::vector<double> &y){
                        if (!impl){
                            throw std::logic_error("predict: model is not fitted yet");
                        }
                        impl->predict(x,y);
                    }
        };
        

    }


    namespace preprocessing {
        // This class generates polynomial features from the input data based on the specified degree and bias inclusion.
        class PolynomialFeatures {
            private:
            // Parameters for polynomial feature generation
                int degree;
                bool include_bias;
            public:
            // Constructor to initialize polynomial feature generator with specified degree and bias inclusion
                PolynomialFeatures(int degree = 2, bool include_bias = false) 
                    : degree(degree), include_bias(include_bias) {
                        // Validate the degree parameter to ensure it is within supported limits
                        if (degree < 1) {
                            throw std::invalid_argument("Polynomial degree must be at least 1");
                    }
                    // For this implementation, we only support degree 1 (linear) and degree 2 (quadratic) features
                        if (degree > 2) {
                            throw std::invalid_argument("Only degrees 1 and 2 are supported");
                        }
                }   

                // Function to transform input feature matrix X into polynomial features based on the specified degree and bias inclusion
                std::vector<std::vector<double>> transform(const std::vector<std::vector<double>>& X) const {
                    if (X.empty() || X[0].empty()) {
                        throw std::invalid_argument("Input data X cannot be empty");
                    }

                    // Validate that all feature columns in X have the same number of samples
                    const size_t n_samples = X[0].size();
                    const size_t n_features = X.size();

                    for (size_t f = 1; f < n_features; ++f) {

                        if (X[f].size() != n_samples) {
                            throw std::invalid_argument("PolynomialFeatures::transform: inconsistent sample size");
                        }
                    }

                    // Generate polynomial features based on the specified degree and bias inclusion
                    std::vector<std::vector<double>> X_poly;

                    // Include bias term (intercept) if specified
                    if (include_bias) {
                        X_poly.push_back(std::vector<double>(n_samples, 1.0));
                    }

                    // Degree 1 terms
                    for (size_t i = 0; i < n_features; ++i) {
                        X_poly.push_back(X[i]);
                    }

                    // Degree 2 terms: x_i * x_j with i <= j
                    if (degree >= 2) {
                        for (size_t i = 0; i < n_features; ++i) {
                            for (size_t j = i; j < n_features; ++j) {
                                std::vector<double> term(n_samples);
                                for (size_t s = 0; s < n_samples; ++s) {
                                    term[s] = X[i][s] * X[j][s];
                                }
                                X_poly.push_back(term);
                            }
                        }
                    }

                    return X_poly;
                }
        };


        // This is preprocessing for the data thats being input. Checking various errors for multiple logistic regression.
        class dataset_checker 
        {
            private:
            // Transpose function for matrix, used as the data is stored as features major, but we need to access it as sample major for the dot product.
                void transpose(std::vector<std::vector<double>>& matrix) {
                    if (matrix.empty() || matrix[0].empty()) {
                        return; // Handle empty matrix case
                    }
                    const size_t rows = matrix.size();
                    const size_t cols = matrix[0].size();
                    std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));
                    for (size_t i = 0; i < rows; ++i) {
                        for (size_t j = 0; j < cols; ++j) {
                            transposed[j][i] = matrix[i][j];
                        }
                    }
                    matrix = std::move(transposed);
                }

            public:
                // Checking each value in data for error.
                void check_data(const std::vector<std::vector<double>>& X, const std::vector<double>& Y,  int num_class)
                {
                    std::vector<std::vector<double>> transposed_X = X; // Create a copy of the feature matrix
                    transpose(transposed_X); // Transpose the feature matrix to access samples as rows

                    check_dimension(transposed_X, Y);

                    for (int i = 0; i < transposed_X.size(); ++i)
                    {
                        for (int j = 0; j < transposed_X[i].size(); ++j)
                        {
                            check_nan_inf( transposed_X[i][j]);
                            check_normalized( transposed_X[i][j]);
                            
                        }
                    }
                }

                // Checks that dimensions match up that all features are the same.
                void check_dimension(const std::vector<std::vector<double>>& X, const std::vector<double>& Y) const
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

        

}
#endif