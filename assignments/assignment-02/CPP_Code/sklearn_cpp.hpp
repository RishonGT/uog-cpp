#ifndef SKLEARN_CPP_HPP
#define SKLEARN_CPP_HPP

#include <iostream>
#include <vector>
#include <cmath>
namespace sklearn_cpp{
namespace linear_model
{
    class LinearRegression {
        private:
            std::vector<double> weights;
            double intercept;

            static double r2_score(
                const std::vector<std::vector<double>>& x,
                const std::vector<double>& y,
                const std::vector<double>& w,
                double b
            ){
                const int m = y.size(); 
                const int n_features = x.size();

                double ss_res = 0.0; // Residual sum of squares
                double ss_tot = 0.0; // Total sum of squares
                double mean_y = 0.0;

                for (double value : y) {
                    mean_y += value;
                }
                mean_y /= m;

                for (int i=0; i < m; i++) {
                    double prediction = b;
                    for (int feature = 0; feature < w.size(); feature++) {
                        prediction += w[feature] * x[feature][i];
                    }
                    ss_res += (y[i] - prediction) * (y[i] - prediction);
                    ss_tot += (y[i] - mean_y) * (y[i] - mean_y);
                }
                return 1 - (ss_res / ss_tot);

            }

            static double mean_func(const std::vector<double>& data) {
                double sum = 0.0;
                for (double value : data) {
                    sum += value;
                }
                return sum / data.size();
            }

            static double std_dev_func(const std::vector<double>& data, double mean) {
                double std_dev = 0.0;
                for (double value : data) {
                    std_dev += (value - mean) * (value - mean);
                }
                return sqrt(std_dev / data.size());
            }

            static std::vector<double> normalise(
                const std::vector<double>& data,
                double& mean,
                double& std_dev
            ) {
                mean = mean_func(data);
                std_dev = std_dev_func(data, mean);

                std::vector<double> normalised_data(data.size());
                for (int i = 0; i < data.size(); i++) {
                    normalised_data[i] = (data[i] - mean) / std_dev;
                }
                return normalised_data;

            }


        public:
            LinearRegression() : intercept(0.0) {}
            
            void fit(std::vector<std::vector<double>>& X, std::vector<double>& Y){
                if (X.empty() || Y.empty() || X[0].size() != Y.size()) {
                    std::cerr << "Invalid input data. Ensure X and Y are non-empty and have compatible dimensions." << std::endl;
                    return;
                }
                
                const int variables = X.size();
                const int m = Y.size(); // Number of data points

                for (int feature = 0; feature < variables; ++feature) {
                    if (static_cast<int>(X[feature].size()) != m) {
                        throw std::invalid_argument("fit: each feature column in X must match Y length");
                    }
                }

                weights.assign(variables, 0.0); // Initialize weights to zero
                double b = 0.0; // Initialize intercept to zero

                std::vector<double> means(variables), std_devs(variables);
                std::vector<std::vector<double>> x_normalised(variables);

                for (int feature = 0; feature < variables; feature++) {
                    x_normalised[feature] = normalise(X[feature], means[feature], std_devs[feature]);
                }

                double mean_y =0.0, std_dev_y = 1.0;

                std::vector<double> y_normalised = normalise(Y, mean_y, std_dev_y);

                std::vector<double> w = weights;
                double learning_rate = 0.01;
                int max_iterations = 10000;
                int iteration = 0;
                const double tolerance = 1e-15;

                double prev_loss = r2_score(x_normalised, y_normalised, w, b);
                
                for (iteration = 0; iteration < max_iterations; iteration++) {
                    std::vector<double> dw(w.size(), 0.0); // Gradient for w
                    double db = 0.0; // Gradient for b

                    // Calculate gradients
                    for (int sample = 0; sample < m; sample++) {
                        double pred = b;
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

                    double current_loss = r2_score(x_normalised, y_normalised, w, b);

                    if (iteration % 500 == 0) { // Print loss every 500 iterations
                        std::cout << "Iteration: " << iteration << ", Loss: " << current_loss << std::endl;
                    }

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
                // b_orig = y_mean + y_std * b_norm - sum_j w_orig[j] * x_mean[j]
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

                std::cout << "Training completed. Final equation: Y = ";
                for (int i = 0; i < weights.size(); i++) {
                    std::cout << weights[i] << " * X" << i + 1;
                    if (i < weights.size() - 1) {
                        std::cout << " + ";
                    }
                }            std::cout << " + " << intercept << std::endl;
                std::cout << "Final Loss: " << prev_loss << std::endl;

            };

            double predict(const std::vector<double>& x) const {
                if (x.size() != weights.size()) {
                    throw std::invalid_argument("predict: input feature vector size must match the number of weights");
                }

                double result = 0;
                for (int i = 0; i < weights.size(); i++) {
                    result += weights[i] * x[i];
                }
                return result + intercept;
            }
        };

};

namespace polynomial_model
{
    class PolynomialFeatures : public linear_model::LinearRegression {
        
} // namespace 

} // namespace sklearn_cpp
#endif