#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <stack>

int main() {
    std::string line;
    std::cout<<"Введите выражение, которое надо посчитать: ";
    std::getline(std::cin, line);
    std::vector<std::string> result;
    std::vector<std::string> numb;
    std::unordered_map<std::string, int> ops = {{"+",1},{"-",1},{"*",2},{"/",2}, {"^",3},{"@",3},{"~",3},{"`",3},{"$",3},{"!",3}};
    std::unordered_map<std::string, std::string> funcs={{"cos","@"}, {"sin","~"}, {"tg","`"}, {"ctg","$"},{"exp","!"}};
    std::string str;
    std::string num="";
    if (line[0]=='-') line[0]='#';
    while (line.find("(-")!=std::string::npos) {
        line.replace(line.find("(-"),2,"(#");
    }
    int chetchik=0;
    for (auto it = funcs.begin(); it != funcs.end(); it++) {
        while (line.find(it->first)!=std::string::npos) {
            auto pos = line.find(it->first);
            line.replace(pos,it->first.length(),"");
            for (int i=pos; i<line.size(); i++) {
                if (line[i]=='(') chetchik++;
                if (line[i]==')') {
                    chetchik--;
                }
                if (chetchik==0) {
                    line.insert(i+1,it->second);
                    break;
                }
            }
        }
    }
    std::string tmp=line;
    std::replace(tmp.begin(), tmp.end(), '+', ' ');
    std::replace(tmp.begin(), tmp.end(), '-', ' ');
    std::replace(tmp.begin(), tmp.end(), '*', ' ');
    std::replace(tmp.begin(), tmp.end(), '/', ' ');
    std::replace(tmp.begin(), tmp.end(), '(', ' ');
    std::replace(tmp.begin(), tmp.end(), ')', ' ');
    std::replace(tmp.begin(), tmp.end(), '^', ' ');
    std::replace(tmp.begin(), tmp.end(), '@', ' ');
    std::replace(tmp.begin(), tmp.end(), '`', ' ');
    std::replace(tmp.begin(), tmp.end(), '~', ' ');
    std::replace(tmp.begin(), tmp.end(), '!', ' ');
    std::replace(tmp.begin(), tmp.end(), '$', ' ');
    tmp.erase(tmp.find_last_not_of(" \n\r\t") + 1);
    std::stringstream ss(tmp);
    std::string nums;
    while (!ss.eof()) {
        ss >> nums;
        std::replace(nums.begin(), nums.end(), '#', '-');
        numb.push_back(nums);
    }

    std::stack<std::string> st;
    int j=0;
    for (int i = 0; i < line.size(); i++) {
        str = line[i];
        if (str == "+" or str == "-" or str == "*" or str == "/" or str== "^" or str=="@" or str=="`" or str=="~" or str=="!" or str=="$") {
            if (st.empty() or st.top() == "(") {
                st.push(str);
            }
            else {
                if (ops[str] <= ops[st.top()]) {
                    while (st.top()!="(" and !st.empty() and ops[str] <= ops[st.top()]) {
                        if (st.top()=="+" or st.top()=="-" or st.top()=="*" or st.top()=="/" or st.top()=="^") {
                            result.push_back(st.top());
                        }
                        else {
                            for (auto it = funcs.begin(); it != funcs.end(); it++) {
                                if (st.top()==it->second) result.push_back(it->first);
                            }
                        }
                        st.pop();
                    }
                    st.push(str);
                }
                else {
                    st.push(str);
                }
            }
        }
        else if (str=="(") st.push(str);
        else if (str==")") {
            while (st.top()!="(") {
                if (st.top()=="+" or st.top()=="-" or st.top()=="*" or st.top()=="/" or st.top()=="^") {
                    result.push_back(st.top());
                }
                else {
                    for (auto it = funcs.begin(); it != funcs.end(); it++) {
                        if (st.top()==it->second) result.push_back(it->first);
                    }
                }
                st.pop();
            }
        }
        else {
            result.push_back(numb[j]);
            i+=numb[j].length()-1;
            j++;
        }
    }
    while (!st.empty()) {
        if (st.top()=="+" or st.top()=="-" or st.top()=="*" or st.top()=="/" or st.top()=="^") {
            result.push_back(st.top());
        }
        else {
            for (auto it = funcs.begin(); it != funcs.end(); it++) {
                if (st.top()==it->second) result.push_back(it->first);
            }
        }
        st.pop();
    }
    std::string xv;
    bool flx = false;
    for (auto const& it : numb) {
        if (it == "x") {
            flx = true;
            break;
        }
    }
    if (flx) {
        std::cout<<"Введите значение переменной: ";std::cin>>xv;
        std::replace(result.begin(), result.end(), std::string("x"), xv);

    }
    std::string dop;
    double num1;
    std::pair<std::string, std::string> pairs;
    for (auto it = result.begin(); it != result.end(); it++) {
        try {
            num1=std::stod(*it);
            if (pairs.first == "") pairs.first = *it;
            else if (pairs.second == "") pairs.second = *it;
            else {pairs.first = pairs.second; pairs.second = *it;}
        }
        catch (...) {
            if (*it=="+" or *it=="-" or *it=="*" or *it=="/" or *it=="^") {
                dop=*it;
                result.erase(it);
                result.erase(--it);
                --it;
                if (dop=="+") *it = std::to_string(std::stod(pairs.first)+std::stod(pairs.second));
                if (dop=="-") *it = std::to_string(std::stod(pairs.first)-std::stod(pairs.second));
                if (dop=="*") *it = std::to_string(std::stod(pairs.first)*std::stod(pairs.second));
                if (dop=="/") *it = std::to_string(std::stod(pairs.first)/std::stod(pairs.second));
                if (dop=="^") *it = std::to_string(pow(std::stod(pairs.first),std::stod(pairs.second)));
                pairs.first = ""; pairs.second = "";
                it=result.begin();
                --it;
            }
            else {
                dop=(*it);
                if (pairs.second!="") num1=std::stod(pairs.second);
                else num1=std::stod(pairs.first);
                result.erase(it); --it;
                if (dop=="cos") *it = std::to_string(cos(num1));
                if (dop=="sin") *it = std::to_string(sin(num1));
                if (dop=="tg") *it = std::to_string(tan(num1));
                if (dop=="ctg") *it = std::to_string(1.0/tan(num1));
                if (dop=="exp") *it = std::to_string(exp(num1));
                pairs.first = ""; pairs.second = "";
                it=result.begin();
                --it;
            }
        }
        if (result.size()<=3) break;
    }
    std::cout<<"Ответ: ";
    if (result.size()==3) {
        if (result[2]=="+") std::cout<<std::stod(result[0])+std::stod(result[1])<<std::endl;
        else if (result[2]=="-") std::cout<<std::stod(result[0])-std::stod(result[1])<<std::endl;
        else if (result[2]=="*") std::cout<<std::stod(result[0])*std::stod(result[1])<<std::endl;
        else if (result[2]=="/") std::cout<<std::stod(result[0])/std::stod(result[1])<<std::endl;
        else if (result[2]=="^") std::cout<<pow(std::stod(result[0]),std::stod(result[1]))<<std::endl;
    }
    if (result.size()==2) {
        if (result[1]=="cos") std::cout<<cos(std::stod(result[0]))<<std::endl;
        else if (result[1]=="sin") std::cout<<sin(std::stod(result[0]))<<std::endl;
        else if (result[1]=="tg") std::cout<<tan(std::stod(result[0]))<<std::endl;
        else if (result[1]=="ctg") std::cout<<1/tan(std::stod(result[0]))<<std::endl;
        else if (result[1]=="exp") std::cout<<exp(std::stod(result[0]))<<std::endl;
    }
}