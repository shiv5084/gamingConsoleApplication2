#include <iostream>
#include <bits/stdc++.h>

using namespace std;

enum BonusType {
    NOBONUS=0,
    SPARE,
    STRIKE,
};

class FrameType {
    int framescore;
    public:
    FrameType():framescore(0){}
    
    virtual void setFrameTrie()=0;
    virtual pair<int,int>& getNormalTries()=0;
    virtual tuple<int,int,int>& getTenthTries()=0;
    void setFrameScore(int score){
        framescore = score;
    }
    virtual ~FrameType()=default;
};

class NormalFrame : public FrameType {
    pair<int,int> tries;
    tuple<int,int,int> tmp;
    int trie1;
    int trie2;
    public:
    NormalFrame(int trie1,int trie2):trie1(trie1),trie2(trie2){
        setFrameTrie();
    }
    void setFrameTrie() override {
        tries = std::make_pair(trie1,trie2);
    }
    pair<int,int>& getNormalTries() override{
        return tries;
    }
    tuple<int,int,int>& getTenthTries() override{   //ony used to define virtual func
        tmp = std::make_tuple(-1,-1,-1);
        return tmp;
    }
};

class TenthFrame : public FrameType {
    tuple<int,int,int> tries;
    pair<int,int> tmp;
    int trie1;
    int trie2;
    int trie3;
    public:
    TenthFrame(int trie1,int trie2,int trie3):trie1(trie1),trie2(trie2),trie3(trie3){
        setFrameTrie();
    }
    void setFrameTrie() override {
        tries = std::make_tuple(trie1,trie2,trie3);
    }
    pair<int,int>& getNormalTries() override{ //ony used to define virtual func
        tmp = std::make_pair(-1,-1);
        return tmp;
    }
    tuple<int,int,int>& getTenthTries() override{
        return tries;
    }
};


class BowlingGame {
    private:
    int numOfFrame;
    BonusType Btype;
    int currentTotalScore;
    int prevTotalScore;
    int totalTriescore;
    vector<FrameType*> frame;
    public:
    BowlingGame(int num):numOfFrame(num),currentTotalScore(0),prevTotalScore(0),Btype(NOBONUS){}
    ~BowlingGame(){
        for(auto fr: frame){
            delete fr;
        }
    }
    
    vector<FrameType*>& getframe(){
        return frame;
    }
    
    int CalculateTotalScore(){
        for(int i=0;i<frame.size();i++){
            if(i < 9){
                totalTriescore = frame[i]->getNormalTries().first + frame[i]->getNormalTries().second;
                currentTotalScore = prevTotalScore+totalTriescore;
                if(frame[i]->getNormalTries().first == 10){
                     Btype = STRIKE;
                    if(i!=8){
                        currentTotalScore = currentTotalScore + frame[i+1]->getNormalTries().first+frame[i+1]->getNormalTries().second;
                    }else{
                        currentTotalScore = currentTotalScore + get<0>(frame[i+1]->getTenthTries())+get<1>(frame[i+1]->getTenthTries());
                    }
                } else if(totalTriescore == 10){
                    Btype = SPARE;
                    if(i!=8){
                        currentTotalScore = currentTotalScore + frame[i+1]->getNormalTries().first;
                    } else {
                        currentTotalScore = currentTotalScore + get<0>(frame[i+1]->getTenthTries());
                    }
                }
            } else {
                totalTriescore = get<0>(frame[i]->getTenthTries())+get<1>(frame[i]->getTenthTries());
                currentTotalScore = prevTotalScore+totalTriescore;
                if(get<0>(frame[i]->getTenthTries()) == 10 || totalTriescore == 10){
                currentTotalScore = currentTotalScore + get<2>(frame[i]->getTenthTries());
                } 
            }
            frame[i]->setFrameScore(currentTotalScore);
            prevTotalScore = currentTotalScore;
        }
        return currentTotalScore;
    }
};

class FrameFactory {
    public:
    static void createFrame(BowlingGame& bg,const string& type,int trie1,int trie2,int trie3=0){
        if(type =="NORMAL"){
            bg.getframe().emplace_back(new NormalFrame(trie1,trie2));
        } else if(type =="TENTH"){
             bg.getframe().emplace_back(new TenthFrame(trie1,trie2,trie3));
        } else {
            throw std::invalid_argument("invalid frame type");
        }
    }
};

int main()
{
    
    BowlingGame player(10);
    vector<int> triesArray = {1,4,4,5,6,4,5,5,10,0,0,1,7,3,6,4,10,0,2,8,6};
    for(int i=0;i<18;i=i+2){
        FrameFactory::createFrame(player,"NORMAL",triesArray[i],triesArray[i+1]);
    }
    FrameFactory::createFrame(player,"TENTH",triesArray[18],triesArray[19],triesArray[20]);
    int totalFinalScore = player.CalculateTotalScore();
    cout<<totalFinalScore;

    return 0;
}

