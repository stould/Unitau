#include <bits/stdc++.h>

using namespace std;

const int MAX_SCORE = 100;// |(0,MAX_SCORE]|
const int MAX_CARDS = 15;//expecting even value
const int UNVISITED = -1;//unvisited states
const int VISITED = 0;//unvisited states

int environment[MAX_CARDS+5], n;

int pd[MAX_CARDS+5][MAX_CARDS+5];

//Dynamic programing computing the game
int getBest(int i, int j){//getting best from smaller states to bigger states
    if (i > j) return 0LL;
    if (i + 1 == j){
        return max(environment[i], environment[j]);
    }
    int& ans = pd[i][j];
    if (ans != -1){
        return ans;
    }
    int l = environment[i] + min(getBest(i + 2, j), getBest(i + 1, j - 1));
    int r = environment[j] + min(getBest(i, j - 2), getBest(i + 1, j - 1));

    return ans = max(l, r);
}

//choosing best move
int choose(int i, int j){
    int l = environment[i] + min(getBest(i + 2, j), getBest(i + 1, j - 1));
    int r = environment[j] + min(getBest(i, j - 2), getBest(i + 1, j - 1));
    return l > r ? 1 : 2;
}

//We will set random environment, in this case
void setRandomEnvironment(){
    int game_cards = rand() % MAX_CARDS + 1;
    if((game_cards & 1) == 1){
        game_cards ^= (1 << 0);
    }
    n = 0;
    for(int i = 0; i < game_cards; i++){
        int score = rand() % MAX_SCORE + 1;
        bool neg = rand() % 2 == 1;
        environment[n++] = neg ? -score : score;
    }
}


/*
  Reading from standard input
  The game expects as valid inputs:
  1 -> Pop card from left and aquire respected scores
  2 -> Pop card from right and aquire respected scores
  There are no other valid inputs.
*/
int readSensor(){
    string input;
    cin >> input;
    if(input.size() != 1){
        return -1;
    }else{
        char ch = input[0];
        if(ch == '1'){
            return 1;
        }else if(ch == '2'){
            return 2;
        }else{
            return -1;
        }
    }
}

void printGame(int l, int r){
    cout << environment[l];
    for(int i = l+1; i <= r; i++){
        cout << " " << environment[i];
    }
    cout << endl;

}

//max 10s
void sleep(double time_){
    const long long factor = 500000000;
    long long W = (long long) (time_ * (double) factor);
    for(long long i = 0; i <= W; i++);
}

void work(){
    srand(time(NULL));//set new seed to rand as actual time -> pseudo RNG
    //setRandomEnvironment();
    cout << "Choose one option:\n1 -> Heads\n2 -> Tails" << endl;
    int parity = -1;
    while(parity == -1){
        parity = readSensor();
        if(parity == -1){
            cout << "Invalid input" << endl;
        }else{
            break;
        }
    }
    int start = rand() % 2;
    cerr << "The result is... ";
    sleep(1.5);
    parity--;
    int turn;
    if(start == 1){
        cout << "Tails";
    }else{
        cout << "Heads";
    }
    if(start == parity){
        turn = 0;

        cout << " .You start to move." << endl;
    }else{
        turn = 1;
        cout << " .Computer starts to move." << endl;
    }
    sleep(2.0);
    cout << endl;
    cout << "Instructions: \'1\' for pop left and \'2\' for pop right" << endl << endl;
    int l = 0, r = (int) n-1;
    int mp = 0, cp = 0;
    cout << "Game initial state: " << endl;
    printGame(l, r);
    getBest(l, r);
    while(l != r){
        if(turn == 0){//human turn
            int option = - 1;
            while(option == -1){
                option = readSensor();
                if(option == -1){
                    cout << "Invalid input" << endl;
                }else{
                    break;
                }
            }
            if(option == 1){
                mp += environment[l];
                l++;
            }else{
                mp += environment[r];
                r--;
            }
            turn ^= 1;
        }else{
            int bestOption = choose(l, r);
            cout << "Computing best move." << endl;
            sleep(1.5);
            if(bestOption == 1){
                cout << "Computer chooses left ( "<< environment[l] << " )." << endl;
                cp += environment[l];
                l++;
            }else{
                cout << "Computer chooses right ( "<< environment[r] << " )." << endl;
                cp += environment[r];
                r--;
            }
            turn ^= 1;
        }
        cout << endl;
        cout << "Game state:" << endl;
        printGame(l, r);
    }
    if((turn & 1) == 1){
        cout << "Computer chooses last card ( "<< environment[l] << " )." << endl;
        cp += environment[l];
    }else{
        cout << "You choose last card ( "<< environment[l] << " )." << endl;
        mp += environment[l];
    }
    cout << endl;
    if(mp > cp){
        cout << "You win!!" << endl;
    }else if(mp < cp){
        cout << "You lose!!" << endl;
    }else{
        cout << "Game draw!!" << endl;
    }
    cout << "My points: " << mp << " " << "Computer points: " << cp << endl;
}

/*
    Note:
    'The number of cards should be even.'
*/
int main(void){
    srand(time(NULL));
    setRandomEnvironment();
    memset(pd,-1,sizeof(pd));
    work();
    return 0;
}
