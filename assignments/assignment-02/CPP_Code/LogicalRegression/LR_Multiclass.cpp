#include "readCSV.hpp"
#include "sklearn_logistical.hpp"

#include <iostream>
#include <sstream>
#include <vector>


int main()
{
    int epochs {1500};
    
    std::vector<double> costs {};
    int class_id = 10;
    for (size_t i = 0; i < class_id; ++i)
    {
        std::cout << "Training class " << i << "\n";
        // Load data for the current class
        ReadCSV numbers("/Users/quantumpip-boy/Documents/Code/Rishons-Code/uog-cpp/assignments/assignment-02/CPP_Code/LogicalRegression/Data/D" + std::to_string(i) + ".csv", ',', 1);
        auto dataframe {numbers.readCSV()};
        
        // To implement a load check ///////////////
        size_t x_cols {dataframe.X[0].size()};

        // Initialize the model for the current class
        linear_model::LogisticalRegressionMulticlass model(x_cols, 0.01, 10);
        
        std::vector<double>& m_weights = model.getWeights();
        double& m_bias = model.getBias();

        for (int j = 0; j < epochs; ++j)
        {
            double final_loss {0.0};
            for (size_t k = 0; k < dataframe.X.size(); ++k)  
                {   
                    costs.clear();
                    const double je = model.xentropyLoss(dataframe.X[k], dataframe.Y, k);
                    costs.push_back(je);
                    const double y_hat = model.predict(dataframe.X[k]);
                    const std::vector<double>& cell{dataframe.X[k]};
                    const double gradient = model.gradientDescent(cell, dataframe.Y, k);
                    m_bias -= 0.01 * gradient;
                    for(size_t i{0}; i < m_weights.size(); ++i)
                    {   
                        m_weights[i] -= 0.03 * gradient * cell[i];
                    }
                    final_loss += je;
                }
            final_loss /= dataframe.X.size();
            const double y_hat = model.predict(dataframe.X[j]);
            // this only goes to 1500 so cant check for anything higher than like class 5 as theres 5k samples at 300 per class.
            if (epochs % 100 == 0)
            {
                std::cout << "Epoch: " << j << " Cost: " << final_loss << " Prediction: " << y_hat << " Y: " << dataframe.Y[j] << " New Bias: " << m_bias << "\n";
            }
            
            //doesnt work i think
            if (costs.size() > 3 && std::abs(final_loss - costs.at(costs.size()-2)) < 1e-8)
                {
                    std::cout << "Convergence reached at iteration " << j << "\n";
                    break;
                }
            else if (j == epochs - 1)
                {
                    std::cout << "End of epochs for class " << final_loss << "\n";
                    break;
                }
        }
        
        
    
    }

    std::cout << "Final costs: \n";
    for (size_t i = 0; i < costs.size(); ++i)
    { std::cout << costs[i] << "\n";
    }
}



