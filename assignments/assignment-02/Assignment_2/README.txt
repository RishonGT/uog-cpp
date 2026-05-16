///////////////////Logistic Regression Multiclass///////////////////

A couple notes worth mentioning. 

Firstly as mnist_full doesnt allow you to put into git as it has a file size of ~350mb which is over the 100mb limit
included is a py program 'data/generate_mnist_csv.py' which generates the data. Followed by a program,
'uog-cpp/assignments/assignment-02/Assignment_2/data/train_data.py' that splits the data into seperate. This was
to avoid issues of psuhing and pulling mnist data. This was done to show whether the model overfits, depending on the size
of the data it can. On mnist micro it does perform potential overfitting(93% on train data and 90% on test) however on mini 
and mnist_full it doesnt seem to. The cost convergence is reached on mnist_full and mini within 50~ epochs. On micro as its much less data
it carries on going and would need a stronger lambda(regularization) value to prevent the overfitting and settle.

With current parameters making it reach convergence at ~29 epochs, converging at ~92% accuracy, 
which from research indicates to be the theoritcal limit for non convolution neural networks for mnist_full dataset.

We loved this project.