// iav180000 - Isabelle Villegas 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath> 
using namespace std;

double sum(vector<double> v){
    double s;

    // Adding each element in the vector to s
    for(int x = 0;x < v.size();x++){
        s+= v[x];
    }
    cout.precision(6);
    return s;
}

double mean(vector<double> v){
    // Using the sum function and diving it by the total 
    // amount of elements in order to get the average
    double m = sum(v)/v.size();
    return m;
}

double median(vector<double> v){
    // Sorting the vector from lowest to highest
    sort(v.begin(), v.end());
    int size = v.size();

    // If the amount of elements in the vector is odd
    // then the middle is taken out and returned as the median
    if(size % 2 == 1){
        return v[size/2];
    }
    // The two middle elements are taken out, added together, and divided to 
    // return median
    else{
        return ((v[size/2] + v[(size/2)+1])/2);
    }
}

void range(vector<double> v){
    // sorting vector to get first and last element
    sort(v.begin(), v.end());
    //vector<reference_wrapper<double>> arr{v.at(0), v.at(v.size()-1)};
    cout.precision(3);

    // printing out the first and last element
    cout << v[0] << "-" << v[v.size()-1] << endl;

}

double covar(vector<double> r, vector<double> m){
    double s = r.size();

    double sum = 0;
    double meanR = mean(r);
    double meanM = mean(m);

    for(int x = 0;x<s;x++){
        sum += (r[x]-meanR) * (m[x]-meanM); 
    }
    
    return  sum/(s-1);
}

double cor(vector<double> r, vector<double> m){
    double s = r.size();

    int sum_r = 0, sum_m = 0, sum_rm = 0;
    int squareSum_r = 0, squareSum_m = 0;
  
    for (int i = 0; i < s; i++)
    {
        sum_r = sum_r + r[i];
        sum_m = sum_m + m[i];
        sum_rm = sum_rm + r[i] * m[i];

        squareSum_r = squareSum_r + r[i] * r[i];
        squareSum_m = squareSum_m + m[i] * m[i];
    }
  
    double corr = (double)(s * sum_rm - sum_r * sum_m) / sqrt((s * squareSum_r - sum_r * sum_r) * (s * squareSum_m - sum_m * sum_m));
  
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