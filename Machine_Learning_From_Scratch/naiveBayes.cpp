#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;


float calcPrior(vector<int> survivedVect){
    //take ratio of survived from vector
    float numPerish = count(survivedVect.begin(), survivedVect.end(), 1);
    float numSurv = count(survivedVect.begin(), survivedVect.end(), 1);

    float perishProb = numPerish / survivedVect.size();
    float survProb = numSurv / survivedVect.size();
    cout << "Prior Probabailities (suvival = no, survival = yes)" << endl;
    cout << "( " << perishProb << ", " << survProb << ")" << endl;

    return numPerish, numSurv, perishProb, survProb;
}

float qualLikelihood(vector <vector<int>> matrixSurv, vector<vector<int>> _classMatr){
    vector<vector<float>> resultVect; 
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < _classMatr[0].size(); j++)
            for(int k = 0; k < matrixSurv[0].size(); k++){
                resultVect[i][j] += matrixSurv[i][k] * _classMatr[k][j];
            }
    //output
    cout << endl << "output" << endl;
    for(int i = 0; i < 2; i++)
    for(int j = 0; j < _classMatr[0].size(); j++){
        cout << " " << resultVect[i][j];
        return resultVect[i][j];
    }
    
}

float likelihoodQuan(vector<int> ageVect){
    //calcualte mean
    float mean = accumulate(ageVect.begin(), ageVect.end(), 0.0) / ageVect.size();
    //calculate variance
    float variance;
    for(int i = 0; i < ageVect.size(); i++){
        variance += ((ageVect[i] - mean) * (ageVect[i] - mean)); 
    }
    variance /= ageVect.size();

    float a = 2.0;
    float b = pow(variance,a);
    //cast agevect to float
    vector<float> floatAge(ageVect.begin(), ageVect.end());
    float y, x;
    for (int i = 0; i < floatAge.size(); i++){
        y = floatAge[i] -= mean;
        x = pow(y, 2);
    }
    float quanLike = 1/(sqrt(2*M_PI*b) * exp(x / (2*variance)));
    return quanLike;

}   

int main(int argc, char** argv){
    
    ifstream inFS; //Input file stream
    string line;
    string heading_in,survived_in, sex_in, pclass_in, age_in;
    const int MAX_LEN = 1050;
    
    vector<int> trainSur;
    vector<int> testSur;
    vector<int> trainSex;
    vector<int> testSex;
    vector<int> trainpClass;
    vector<int> testpClass;
    vector<int> trainAge;
    vector<int> testAge;
    vector<int> heading(MAX_LEN);
    
    //Try to open file
    cout << "Opening file tiatanic_project.csv." << endl;

    inFS.open("titanic_project.csv");
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
    while (inFS.good() && numObservations < 801) {
        getline(inFS, heading_in, ',');
        getline(inFS, pclass_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in);

        trainSur.at(numObservations) = stof(survived_in);
        trainSex.at(numObservations) = stof(sex_in);
        trainpClass.at(numObservations) = stof(pclass_in);
        trainAge.at(numObservations) = stof(age_in);
        numObservations++;
    }
    

    int numObservationsTest = 801;
        while (inFS.good() && numObservations < 1045) {
            getline(inFS, heading_in, ',');
            getline(inFS, pclass_in, ',');
            getline(inFS, survived_in, ',');
            getline(inFS, sex_in, ',');
            getline(inFS, age_in);

            testSur.at(numObservationsTest) = stof(survived_in);
            testSex.at(numObservationsTest) = stof(sex_in);
            testpClass.at(numObservationsTest) = stof(pclass_in);
            testAge.at(numObservationsTest) = stof(age_in);
            numObservationsTest++;
    }

    //create matrices for mulitplication in other methods (quanlitative)
    //TODO - separate into train/test stuffs.
    vector<vector<int>> survivalMatrixTrain(2, vector<int> (numObservations));
    vector<vector<int>> survivalMatrixTest(2, vector<int> (numObservationsTest));
    vector<vector<int>> pclassMatrTrain(numObservations, vector<int> (3));
    vector<vector<int>> pclassMatrTest(numObservationsTest, vector<int> (3));
    vector<vector<int>> sexMatrTrain(numObservations, vector<int> (2));
    vector<vector<int>> sexMatrTest(numObservationsTest, vector<int> (2));

    //TODO = function calls for train/test and clean up 

    //prior prob calculations
    calcPrior(trainSur);
    calcPrior(testSur);
    //likelihood calls TRAIN
    qualLikelihood(survivalMatrixTrain, pclassMatrTrain);
    qualLikelihood(survivalMatrixTrain, sexMatrTrain);
    //likelihood calls TEST
    qualLikelihood(survivalMatrixTest, pclassMatrTest);
    qualLikelihood(survivalMatrixTest, sexMatrTest);

    //Quanitiative likelihoodcalls
    likelihoodQuan(trainAge);
    likelihoodQuan(testAge);
        
    float trainFinal = (qualLikelihood(survivalMatrixTrain, pclassMatrTrain) * qualLikelihood(survivalMatrixTrain, sexMatrTrain)
    * likelihoodQuan(trainAge) * calcPrior(trainSur));
    float testFinal = (qualLikelihood(survivalMatrixTest, pclassMatrTest) * qualLikelihood(survivalMatrixTest, sexMatrTest)
    * likelihoodQuan(testAge) * calcPrior(testSur));

    cout << trainFinal << endl;
    cout << testFinal << endl;
}