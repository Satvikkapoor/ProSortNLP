// Project UID db1f506d06d84ab787baf250c265e24e
#include "csvstream.h"
#include <map>
#include <set>
#include <cmath>
#include <iostream>
using namespace std;




class Classifier{

public:
    Classifier(){
        posts = 0;
    }

    // EFFECTS: Return a set of unique whitespace delimited words.
    set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
            words.insert(word);
        }
        return words;
    }

    double log_prior(int postwithlabel){
        double val =  static_cast<double>(postwithlabel) / posts;
        return (log(val));
    }

    double log_(int wordcountlabel, int postwithlabel, int postwithword){
        double val;
        if(wordcountlabel > 0){
            val = static_cast<double>(wordcountlabel) / postwithlabel;
            return (log(val));
        }
        else if(wordcountlabel == 0 && postwithword > 0){
            val = static_cast<double>(postwithword) / posts;
            return (log(val));
        }
        else if(wordcountlabel == 0 && postwithword == 0){
            val = static_cast<double>(1) / posts;
            return (log(val));
        }
        return 0.0;
    }

    void train(csvstream &train, bool debug){
        vector<pair<string, string>> row;
        if(debug){
            cout << "training data:" << endl;
        }
        while(train >> row){
            int findtag;
            for(int i = 0; i < row.size(); i++){
                if(row[i].first == "tag"){
                    findtag = i;
                }
            }

            for (unsigned int i = findtag; i < row.size(); ++i) {
                //either gives me tags or the content 
                //even i gives tags and odd i gives content 
                string &alltext = row[i].second;
                if(i % 2 == 0){
                    if(debug){
                        cout << "  label = " << alltext << ", ";
                    }
                    //alltext has tag names
                    post_with_label[alltext]++;
                }
                else if(i % 2 != 0){
                    if(debug){
                        cout << "content = " << alltext << endl;
                    }
                    set<string> words = unique_words(alltext);
                    for(auto find : words){
                        //alltext has the content 
                        post_with_w[find]++;
                    }
                }
            }
            posts++;
            set<string> content = unique_words(row[findtag + 1].second);
            for(const string &word: content){
                post_with_label_word[row[findtag].second][word]++;
            }
        }
        cout << "trained on " << posts << " examples" << endl;
        if(debug){
            cout << "vocabulary size = " << post_with_w.size() << endl;
        }
        cout << endl;
        if(debug){
            int nwords;
            cout << "classes:" << endl;
            for(auto labels: post_with_label){
                cout << "  " << labels.first << ", " << labels.second << " examples, ";
                cout << "log-prior = " << log_prior(labels.second) << endl;
            }
            cout << "classifier parameters:" << endl;
            for(auto label : post_with_label_word){
                for(auto words : label.second){
                    nwords = post_with_w[words.first];
                    cout << "  " << label.first << ":" << words.first;
                    cout << ", count = " << words.second << ", log-likelihood = ";
                    cout <<
                        log_(words.second,post_with_label[label.first],nwords); 
                    cout << endl;
                }
            }
            cout << endl;
        }
    }

    pair<string,double> predict(set<string> content){
        pair<string, double> prediction;
        double finalval = 0.0;
        for(auto i : post_with_label){
            double val = log_prior(i.second);
            for(string words : content){
                if(post_with_label_word[i.first][words] != 0){
                    val+=log((double)(post_with_label_word[i.first][words]) 
                               / post_with_label[i.first]);
                }
                else if(post_with_w[words] != 0){
                    val += log(static_cast<double>(post_with_w[words]) / posts);
                }
                else{
                    val += log(1.0 / posts);
                }
            }
            if(finalval == 0){
                finalval = val;
            }
            if(max(finalval, val) == val){
                finalval = val;
                prediction.first = i.first;
                prediction.second = val;
            }
        } 
        return prediction;  
    }

    void test(csvstream &train){
        cout << "test data:" << endl;
        vector<pair<string, string>> row;
        int score = 0;
        int counttest = 0;
        while(train >> row){
            int findtag;
            for(int i = 0; i < row.size(); i++){
                if(row[i].first == "tag"){
                    findtag = i;
                }
            }
            string correctlabel = row[findtag].second;
            string &alltext = row[findtag + 1].second;
            set<string> unq = unique_words(alltext);
            pair<string, double> result = predict(unq);

            cout << "  correct = " << correctlabel << ", predicted = " << result.first;
            cout << ", log-probability score = " << result.second << endl;
            cout << "  content = " << alltext << endl << endl;

            if(correctlabel == result.first){
                score++;
            }
            counttest++;
        }  
        cout << "performance: ";
        cout << score << " / " << counttest << " posts predicted correctly"; 
        cout << endl;
    }

private:
    //total posts in set
    int posts;

    //number of posts that contain word 
    map<string, int> post_with_w;

    //number of posts with that label
    map<string, int> post_with_label;

    //the number of posts with label that contain 
    map<string, map<string, int>> post_with_label_word;

    //stores log prior 
    map<string, double> label_log;

    //stores log-likelihood label, word, log
    map<string, map<string, double>> word_label_log;
};


int main(int argc, const char * argv[]) {
    cout.precision(3);
    if(argc != 3 && argc != 4){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    if(argc == 4 && strcmp(argv[3], "--debug")){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    string train = argv[1];
    string test = argv[2];

    try {
         csvstream csv_train(train , ',', false);
    }
    catch(const csvstream_exception &e){
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    try {
         csvstream csv_test(test, ',', false);
    }
    catch(const csvstream_exception &e){
        cout << "Error opening file: " << argv[2] << endl;
        return 1;
    }
    csvstream csv_train(train , ',', false);
    csvstream csv_test(test, ',', false);

    bool debug;
    if(argc == 4){
        debug = true;
    }
    else{
        debug = false;
    }

    Classifier run;
    run.train(csv_train, debug);
    run.test(csv_test);


    return 0;
}
