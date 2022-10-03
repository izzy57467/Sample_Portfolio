#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std::chrono;
using namespace std;

double accuracy(int tp, int fn, int tn, int fp){
    double result = (double)(tp+tn)/(tp+tn+fp+fn);
    return result;
}

double sensitivity(int tp, int fn, int tn, int fp){
    double result = ((double)tp/(tp+fn));
    return result;
}

double specificity(int tp, int fn, int tn, int fp){
    double result = ((double)tn/(tn+fp));
    return result;
}

bool abs_sort(double x, double y)
{
    double  x1 = abs(x-0);
    double  y1 = abs(y-0);
    double result = x1 < y1;
    return result;
}

int main(int argc, char** argv){
    
    ifstream inFS; //Input file stream
    string line;
    string age_in,survived_in, sex_in, pclass_in;

    // Setting length of divide
    const int TRAIN_LEN = 800;
    const int TEST_LEN = 250;

    // Training data frame
    vector<int> train_survived(TRAIN_LEN);
    vector<int> train_sex(TRAIN_LEN);
    
    // Testing data frame
    vector<int> test_survived(TEST_LEN);
    vector<int> test_sex(TEST_LEN);

    //Try to open file
    cout << "Opening file titanic_project.csv." << endl;

    inFS.open("titanic.csv");
    if(!inFS.is_open()){
        cout << "Could not open file titanic_project.csv." << endl;
        return 1; //1 indicates error
    }

    //Can now use inFS stream like cin stream
    //Boston.csv should contain two doubles

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    //echo heading
    cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good() && numObservations < 800) {
        getline(inFS, pclass_in, ',');
       // cout << heading_in << endl;
        getline(inFS, survived_in, ',');
       // cout << pclass_in << endl;
        getline(inFS, sex_in, ',');
       // cout << survived_in << endl;
        getline(inFS, age_in);
       // cout << sex_in << endl;

        train_survived.at(numObservations) = stof(survived_in);
        train_sex.at(numObservations) = stof(sex_in);

        numObservations++;
    }

    numObservations = 0;
    while (inFS.good() && numObservations < 250) {
        getline(inFS, pclass_in, ',');
       // cout << heading_in << endl;
        getline(inFS, survived_in, ',');
       // cout << pclass_in << endl;
        getline(inFS, sex_in, ',');
       // cout << survived_in << endl;
        getline(inFS, age_in);
       // cout << sex_in << endl;

        test_survived.at(numObservations) = stof(survived_in);
        test_sex.at(numObservations) = stof(sex_in);

        numObservations++;
    }
    
    ofstream myfile;
    myfile.open("results.txt");

    vector<double> error; // for storing the error values
    vector<double> output;
    double p, pred, err, x = 0, y = 0, alpha = 0.01;

    // STARTING CLOCK
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // TRAINING THE DATA
    for (int i = 0; i < 800; i++) { 
    p = -(x + y * train_sex.at(i)); 
    pred  = 1/(1+ exp(p)); // making the prediction
    err = abs(train_survived.at(i)-pred);  // calculating the error
    x = x - alpha * err * pred * (1-pred) * 1.0; // reevaluating x
    y = y + alpha * err * pred * (1-pred) * train_sex.at(i); // reevaluating y
    output.push_back(abs(y));
    myfile << "Coefficients: " << "x = " << abs(x) << " " << "y = " << abs(y) << " " << " Error = " << err << " Probability:" << pred << endl; // printing values after every step
    error.push_back(err);
    }

    // ENDING CLOCK
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  
    std::cout << "Elapsed time during training: " << elapsed_seconds.count() << "s\n";
    

    sort(error.begin(),error.end(),abs_sort); // sorting to get the minimum value
    cout<< "Final Coefficients are: " << "x = " << abs(x) << " " << "y = " << abs(y) << " " << "error = " << error[0] << endl;

    // TESTING PHASE
    double v; // ENTER EXAMPLE VALUE
    cout << "Enter a value: " << endl;
    cin >> v;
    pred = abs(x + y * v); // CALCULATES THE PREDICTION

    cout << "The value predicted by the model= "<< pred <<endl;
    
    // WILL DETERMINE WHETHER OR NOT THEY SURVIVED BASED ON THE PERCENTAGE FOR SEX 
    if(pred > 0.5){
        pred = 1;
    }
    else{
        pred = 0;
    }

    cout  << "The class predicted by the model = " << pred << endl;

    int TP, FN, TN, FP; // VARIABLES TO CALCULATE ACCURACY, SENSITIVITY, AND SPECIFICITY

    TP++;
    for(int x = 0;x<output.size();x++){
        if(abs(output.at(x)) > 0.5 && train_survived.at(x) == 1){
            TP++;
        }
        if(abs(output.at(x)) > 0.5 && train_survived.at(x) == 0){
            FN++;
        }
        if(abs(output.at(x)) < 0.5 && train_survived.at(x) == 0){
            TN++;
        }
        if(abs(output.at(x)) < 0.5 && train_survived.at(x) == 1)
            FP++;
    }

    /*
    cout << "TP: " << TP << endl;
    cout << "FN: " << FN << endl;
    cout << "TN: " << TN << endl;
    cout << "FP: " << FP << endl;
    */

    cout << "Accuracy: " << accuracy(TP, FN, TN, FP) << endl;
    cout << "Sensitivity: " << sensitivity(TP, FN, TN, FP) << endl;
    cout << "Specificity: " << specificity(TP, FN, TN, FP) << endl;

    cout << "Closing file Boston.csv." << endl;
    inFS.close(); // CLOSING FILE 
    myfile.close(); // CLOSING WRITING TO RESULTS FILE
}