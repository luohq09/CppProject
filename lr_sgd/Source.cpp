#include <vector>
#include <unordered_map>
#include <utility>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

typedef int idxT;
typedef double dataT;
typedef double weightT;

#define stoid stoi
#define stodata stod

using namespace std;

bool rand_weight = false;
double initial_learning_rate = 0.01;
bool learning_rate_decay = false;
double learning_rate_decay_exp_base = 2.0;
int max_iteration_num = 1000;
bool need_shuffle_data = false;
size_t mini_batch_size = 1;
double l1_penalty_weight = 0.0001;

random_device rd;
mt19937 e2(rd());
uniform_real_distribution<> dist(-1, 1);

inline weightT init_weight() {
    if (rand_weight) {
        return dist(e2);
    } else {
        return (weightT)0;
    }
}

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

inline vector<pair<idxT, dataT>> parse_line(const string &line) {
    vector<pair<idxT, dataT>> res;
    vector<string> elems = split(line, ' ');
    if (!elems.empty()) {
        int label = stoi(elems.front());
        res.push_back(make_pair(0, (dataT)label));
        for (auto it = elems.begin() + 1; it != elems.end(); ++it) {
            vector<string> idx_val = split(*it, ':');
            if (idx_val.size() == 2) {
                res.push_back(make_pair(stoid(idx_val[0]), stodata(idx_val[1])));
            }
        }
    }

    return res;
}

vector<vector<pair<idxT, dataT>>> get_training_Data(const string &train_file) {
    ifstream ifs(train_file);
    vector<vector<pair<idxT, dataT>>> data;
    if (ifs.good()) {
        string line;
        while (getline(ifs, line)) {
            auto data_ins = parse_line(line);
            if (data_ins.size() > 1) {
                data.push_back(move(data_ins));
            }
        }
    }

    return data;
}


inline weightT sigmoid(weightT x) {
    return 1 / (1 + exp(-x));
}

weightT predict(unordered_map<idxT, weightT> &weights, const vector<pair<idxT, dataT>> &data_ins) {
    weightT sum = 0;
    for (auto fit = data_ins.begin(); fit != data_ins.end(); ++fit) {
        auto weight_it = weights.find(fit->first);
        if (weight_it != weights.end()) {
            sum += fit->second * weight_it->second;
        }
    }

    return sigmoid(sum);
}

vector<pair<idxT, weightT>> 
mini_batch_gradient(
    unordered_map<idxT, weightT> &weights,
    const vector<vector<pair<idxT, dataT>>> &data,
    const vector<size_t> &data_idx
)
{
    const size_t dataLen = data_idx.size();
    unordered_map<idxT, weightT> gradients;
    for (auto it = data_idx.begin(); it != data_idx.end(); ++it) {
        const vector<pair<idxT, dataT>> &data_ins = data[*it];
        int label = data_ins.front().second == 1 ? 1 : 0;
        weightT sum = 0;
        for (auto fit = data_ins.begin() + 1; fit != data_ins.end(); ++fit) {
            // predict & initialize weight
            idxT idx = fit->first;
            auto weight_it = weights.find(idx);
            weightT weight;
            if (weight_it == weights.end()) {
                weight = init_weight();
                weights.insert(make_pair(idx, weight));
            }
            else {
                weight = weight_it->second;
            }

            sum += fit->second * weight;
        }
        weightT prob = sigmoid(sum);
        for (auto fit = data_ins.begin(); fit != data_ins.end(); ++fit) {
            gradients[fit->first] += ((label - prob) * fit->second);
        }
    }

    vector<pair<idxT, weightT>> res;
    res.reserve(gradients.size());
    for (auto it = gradients.begin(); it != gradients.end(); ++it) {
        res.push_back(make_pair(it->first, it->second / dataLen));
    }

    return res;
}

void apply_cumulative_l1_penalty(
    unordered_map<idxT, weightT> &weights,
    const vector<pair<idxT, weightT>> &gradients,
    weightT learning_rate,
    weightT cumulative_l1_penalty,
    unordered_map<idxT, weightT> &cumulative_l1_actual_penalty) {

    for (auto gradient = gradients.begin(); gradient != gradients.end(); ++gradient) {
        idxT idx = gradient->first;
        weightT gradient_val = gradient->second;
        auto weight = weights.find(idx);
        weightT half_update_weight = weight->second + gradient_val * learning_rate;
        auto last_actual_penalty = cumulative_l1_actual_penalty.find(idx);
        if (last_actual_penalty == cumulative_l1_actual_penalty.end()) {
            last_actual_penalty = cumulative_l1_actual_penalty.insert(make_pair(idx, 0.0)).first;
        }
        if (half_update_weight > 0) {
            weight->second = max(0.0, half_update_weight - (cumulative_l1_penalty + last_actual_penalty->second));
        } else if (half_update_weight < 0) {
            weight->second = max(0.0, half_update_weight + (cumulative_l1_penalty - last_actual_penalty->second));
        }

        last_actual_penalty->second += weight->second - half_update_weight;
    }
}

double calculate_loss(const unordered_map<idxT, weightT> &weights, const vector<vector<pair<idxT, dataT>>> &data) {
    weightT loss = 0;
    for (auto it = data.begin(); it != data.end(); ++it) {
        int label = it->front().second == 1 ? 1 : 0;
        weightT sum = 0;
        for (auto fit = it->begin() + 1; fit != it->end(); ++fit) {
            auto weight_it = weights.find(fit->first);
            if (weight_it != weights.end() && weight_it->second != 0) {
                sum += weight_it->second * fit->second;
            }
        }
        if (label == 1) {
            sum = -sum;
        }
        loss += log(1 + exp(sum));
    }

    weightT l1_val = 0;
    for (auto it = weights.begin(); it != weights.end(); ++it) {
        l1_val += abs(it->second);
    }

    return loss / data.size() + l1_val;
}

void train(const string &in_data_file, const string &out_model_file) {
    auto data = get_training_Data(in_data_file);
    if (data.empty()) return;

    const size_t data_len = data.size();
    double learning_rate = initial_learning_rate;
    int k = 1; // traning iterations;
    if (mini_batch_size < 0 || mini_batch_size > data_len) mini_batch_size = data_len;

    vector<size_t> data_idx;
    data_idx.reserve(data_len);
    for (size_t i = 0; i < data_len; ++i) {
        data_idx.push_back(i);
    }

    unordered_map<idxT, weightT> weights;
    double cumulative_l1_penalty = 0.0;
    unordered_map<idxT, weightT> cumulative_l1_actual_penalty;
    weightT loss = 0.0;
    const weightT epsilon = 0.001;
    for (int i = 0; i < max_iteration_num; ++i) {
        if (need_shuffle_data) {
            shuffle(data_idx.begin(), data_idx.end(), e2);
        }

        for (size_t i = 0; i < data_len; i += mini_batch_size) {
            size_t begin = i;
            size_t end = min(i + mini_batch_size, data_len);
            vector<size_t> mini_batch_data_idx(data_idx.begin() + begin, data_idx.begin() + end);
            auto gradients = mini_batch_gradient(weights, data, mini_batch_data_idx);

            // learning rate
            if (learning_rate_decay) {
                learning_rate = pow((double)learning_rate_decay_exp_base, -(double)k / data_len) * initial_learning_rate;
            }

            cumulative_l1_penalty += learning_rate * l1_penalty_weight;
            apply_cumulative_l1_penalty(weights, gradients, learning_rate, cumulative_l1_penalty, cumulative_l1_actual_penalty);
            ++k;
        }

        weightT new_loss = calculate_loss(weights, data);
        if (abs(new_loss - loss) < epsilon) break;
        loss = new_loss;
    }
}