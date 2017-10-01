

double function2(double x){
    return x*x + -2*x + 4;
}
double function1(double x){
    return x + 2;
}

vector<double> newScale(double factor, vector<int> &oldFactor){
    vector<double> ans;
    for(int i = 0; i < oldFactor.size(); ++i){
        ans.push_back(oldFactor[i] * factor);
    }
    return ans;
}

vector<double> sum(double start, double increment, double stop){
    vector<double> ans;
    for(double t = start; t <= stop; t+= increment){
        ans.push_back(function1(t) + function2(t));
    }
    return ans;
}
