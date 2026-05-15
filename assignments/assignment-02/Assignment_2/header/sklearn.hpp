#ifndef SKLEARN_CPP_HPP
#define SKLEARN_CPP_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>
#include <set>


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
                    const int m = y.size(); 

                    double ss_res  {0.0}; // Residual sum of squares
                    double ss_tot  {0.0}; // Total sum of squares
                    double mean_y  {0.0};

                    mean_y = mean_func(y);

                    // Calculate predictions and sums for R² score
                    for (int i=0; i < m; i++) {
                        double prediction = b;
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
                    const int m = y.size(); 
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

                    double Y_std = std_dev_func(Y, mean_func(Y));
                    // Check for zero variance in the target variable
                    if (Y_std == 0.0) {
                        std::cerr << "All target values are the same. The model cannot be fitted." << std::endl;
                        return false;
                    }

                    // Check for zero variance in each feature
                    for (const std::vector<double>& feature_column : X) {
                        double feature_std = std_dev_func(feature_column, mean_func(feature_column));
                        if (feature_std == 0.0) {
                            std::cerr << "One of the features has zero variance. The model cannot be fitted." << std::endl;
                            return false;
                        }
                    }

                    // Validate that each feature column in X has the same number of samples as Y
                    const int variables = X.size();
                    const int m = Y.size(); // Number of data points

                    for (int feature = 0; feature < variables; ++feature) {
                        if (static_cast<int>(X[feature].size()) != m) {
                            throw std::invalid_argument("fit: each feature column in X must match Y length");
                        }
                    }


                    weights.assign(variables, 0.0); // Initialize weights to zero
                    double b = 0.0; // Initialize intercept to zero

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

                    std::vector<double> y_normalised = normalise(Y, mean_y, std_dev_y);

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
                    double prev_loss {0.0};
                    prev_loss = loss_function(x_normalised, y_normalised, w, b);

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
                            
                            double residual = pred - y_normalised[sample];
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
        /*This class calls the correct binary or multiclassification class depending on label sizes*/    
        class LogisticalRegression {
            private:


                void configureRegression(
                    const std::vector<double> &y
                )
                {
                    std::set<double> unquie_labels(y.begin(), y.end());
                }



            public:

                void fit(
                    const std::vector<std::vector<double>> &x,
                    const std::vector<double> &y
                )
                {
                    LogisticalRegressionBinary log_reg;
                    log_reg.fit(x,y);
                }

        };
        class LogisticalRegressionBinary
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

    }


    namespace preprocessing {
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
    }

}
#endif