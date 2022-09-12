// iav180000 - Isabelle Villegas 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath> 
using namespace std;

double sum(vector<double> v){
    double s;
    for(int x = 0;x < v.size();x++){
        s+= v[x];
    }
    cout.precision(6);
    return s;
}

double mean(vector<double> v){
    double m = sum(v)/v.size();
    return m;
}

double median(vector<double> v){
    sort(v.begin(), v.end());
    int size = v.size();
    if(size % 2 == 0){
        return v[size/2];
    }
    else{
        return ((v[size/2] + v[(size/2)+1])/2);
    }
}

void range(vector<double> v){
    sort(v.begin(), v.end());
    //vector<reference_wrapper<double>> arr{v.at(0), v.at(v.size()-1)};
    cout.precision(3);
    cout << v[0] << "-" << v[v.size()-1] << endl;

}

double covar(vector<double> r, vector<double> m){
    double sum = 0;
    double meanR = mean(r);
    double meanM = mean(m);

    for(int x = 0;x<r.size();x++){
        sum += (r[x]-meanR) * (m[x]-meanM); 
    }
    
    return  sum/(r.size()-1);
}

double cor(vector<double> X, vector<double> Y){
    double n = X.size();

    int sum_X = 0, sum_Y = 0, sum_XY = 0;
    int squareSum_X = 0, squareSum_Y = 0;
  
    for (int i = 0; i < n; i++)
    {
        // sum of elements of array X.
        sum_X = sum_X + X[i];
  
        // sum of elements of array Y.
        sum_Y = sum_Y + Y[i];
  
        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[i] * Y[i];
  
        // sum of square of array elements.
        squareSum_X = squareSum_X + X[i] * X[i];
        squareSum_Y = squareSum_Y + Y[i] * Y[i];
    }
  
    // use formula for calculating correlation coefficient.
    double corr = (double)(n * sum_XY - sum_X * sum_Y) / sqrt((n * squareSum_X - sum_X * sum_X) * (n * squareSum_Y - sum_Y * sum_Y));
  
    cout.precision(2);
    return corr;
}

// Printing Stats out for a vector will print out Size, Sum, Mean, Median, and Range
void print_stats(vector<double> v){
    cout << "\n Number of Records: " << v.size() << endl;
    cout << "\n Sum = " << sum(v) << endl;
    cout << "\n Mean = " << mean(v) << endl;
    cout << "\n Median = " << median(v) << endl;
    cout << "\n Range = ";
    range(v);
    cout << endl;
}

/////////////////////////////////////////////////////////

int main(int argc, char** argv){
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    //Try to open file
    cout << "\nOpening file Boston.csv." << endl << endl;
    
    inFS.open("Boston.csv");
    if(!inFS.is_open()){
        cout << "Could not open file Boston.csv" << endl << endl;
        return 1; // 1 indicates error
    }

    getline(inFS, line);

    //echo heading
    cout << "heading: " << line << endl << endl;

    int numObservations = 0;
    while(inFS.good()){

        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "Closing file Boston.csv" << endl << endl << endl;
    inFS.close(); // Done with file, so close it

    cout << "Stats for rm" << endl;
    print_stats(rm);

    cout << "\nStats for medv" << endl;
    print_stats(medv);


    cout << "Covariance = " << covar(rm, medv) << endl;
    cout << "\nCorrelation = " << cor(rm, medv) << endl;

    cout << "\nProgram terminated." << endl;
    
    return 0;


}