#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int lastStone;
int num_of_stones;
int num_of_taken_stones;
int* list_of_taken_stones;
int* remainning_stone;
bool player;

int bestMove;
double calculatedValue;
int numOfVisitedNodes;
int maxDepth;

double max = -10000;
double min = -10000;

int position;

void update_remainning_stone(); 
double evaluation(int nextStone, bool player);
void alphaBetaPrunning();

int main(int argc, char* argv[]) {

    int i, j=0;

    num_of_stones = atoi(argv[1]); // 게임에 주어진 전체 돌의 개수
    num_of_taken_stones; // 가져간 돌의 개수
    if (argc > 1) num_of_taken_stones = atoi(argv[2]);
    else num_of_taken_stones = 0; // 가져간 돌이 없으면 0개로 저장
    list_of_taken_stones = malloc(sizeof(int) * num_of_stones); // 가져간 돌의 리스트
    if (argc > 1) {
        for( i=0; i<num_of_taken_stones; i++)
            list_of_taken_stones[i] = atoi(argv[i+3]);
        lastStone = atoi(argv[i+2]); //마지막 돌 저장
    } //가져간 돌을 리스트에 저장
    if(num_of_taken_stones == 0 || num_of_taken_stones % 2 == 0)
        player = true; //가져간 돌의 개수가 짝수거나 0개이면 max(true)시작
    else player = false;
    remainning_stone = malloc(sizeof(int) * num_of_stones + 1); //남아있는 돌
    for (i=0; i<num_of_stones; i++) remainning_stone[i] = i+1;
    update_remainning_stone();
    int* possible_to_take = malloc(sizeof(int) * num_of_stones);

    if(num_of_taken_stones == 0) { // 가져간 돌이 하나도 없는 경우
        for(i=1; i<num_of_stones; i++) {
            if(i % 2 == 1) {
                possible_to_take[j] = i;
                j++;
            }
        }
        for(i=0; i<num_of_stones; i++) {
            if(evaluation(possible_to_take[i], true) > max) { 
                max = evaluation(possible_to_take[i], true);
                position = i;
            }
        }
        list_of_taken_stones[0] = possible_to_take[position];
        num_of_taken_stones++;
        update_remainning_stone();
        bestMove = possible_to_take[position];
        calculatedValue = max;
        numOfVisitedNodes = j - 1;
        maxDepth++;
        player = false;
    }

    alphaBetaPrunning();
    
    printf("BestMove: %d\n", bestMove);
    if(player) calculatedValue = -1.0;
    else calculatedValue = 1.0;
    printf("Calculated Value: %f\n", calculatedValue);
    printf("Number of Visited Node: %d\n", numOfVisitedNodes);
    printf("Max Depth: %d\n", maxDepth);

    return 0;
}
void alphaBetaPrunning() {
    update_remainning_stone();
    int i, j = 0, index;
    int lastStone = list_of_taken_stones[num_of_taken_stones-1];
    int* nextMove = malloc(sizeof(int)* num_of_stones);
    double* nextEvaluation = malloc(sizeof(double) * num_of_stones);
    max = -10000;
    min = -10000;
    if(player) {
        for(i=0; i<num_of_stones; i++) {
            if(remainning_stone[i] != 0 && lastStone % remainning_stone[i] == 0) {
                nextMove[j] = remainning_stone[i];
                j++; //다음으로 선택할 수 있는 돌들을 저장
                if(j == 0) return;
            }
        }
        for(i=0; i<j; i++) {
            nextEvaluation[i] = evaluation(nextMove[i], true);
            if (nextEvaluation[i] > max) {
                max = nextEvaluation[i];
                index++;
            }
        }
        bestMove = nextMove[index];
        calculatedValue = max;
        numOfVisitedNodes += j-1;
        maxDepth++;
        list_of_taken_stones[num_of_taken_stones] = bestMove;
        num_of_taken_stones++;
        player = false;

        alphaBetaPrunning();
    }
    else if(!player) {
        for(i=0; i<num_of_stones; i++) {
            if(remainning_stone[i] != 0 && remainning_stone[i] % lastStone == 0) {
                nextMove[j] = remainning_stone[i];
                j++; //다음으로 선택할 수 있는 돌들을 저장
                if(j == 0) return;
            }
        }
        for(i=0; i<j; i++) {
            nextEvaluation[i] = evaluation(nextMove[i], true);
            if (nextEvaluation[i] > max) {
                max = nextEvaluation[i];
                index++;
            }
        }
        bestMove = nextMove[index];
        calculatedValue = max;
        numOfVisitedNodes += j-1;
        maxDepth++;
        list_of_taken_stones[num_of_taken_stones] = bestMove;
        num_of_taken_stones++;
        player = true;

        alphaBetaPrunning();
    }
}
void update_remainning_stone() {
    //남아있는 돌을 갱신해주는 함수
    int i;
    for(i=0; i<num_of_stones; i++)
        remainning_stone[list_of_taken_stones[i]-1] = 0;
}

double evaluation(int nextStone, bool player) {
    //돌을 evaluation값으로 변경해주는 함수
    int i, cnt;
    int* temp = malloc(sizeof(int) * num_of_stones);
    for(i=0; i<num_of_stones; i++) temp[i] = remainning_stone[i];
    if (player) {
        if(nextStone != 1 && remainning_stone[1] ==1) return 0;
        // 1번 돌을 아직 가지고 가지 않는 경우 0반환
        else if(nextStone == 1) { // 마지막에 1을 가지고 간 경우
            temp[1] = 0; 
            for(i=0; i<num_of_stones; i++)
                if(temp[i] != 0) cnt++;
            if (cnt % 2 == 0) return -0.5; // 짝수면 -0.5반환
            else return 0.5; // 홀수는 0.5
        }
        else if(lastStone % nextStone == 0) {  // 마지막에 소수를 가지고 간 경우
            if((num_of_stones / nextStone) % 2 == 0) return -0.7; //짝수인 경우
            else return 0.7; // 홀수인 경우
        }
        else if(lastStone % nextStone != 0) { // 마지막에 소수가 아닌 돌을 가지고 간 경우
            cnt = 0;
            for(i=0; i<nextStone; i++)
                if(nextStone % i == 0) cnt++;
            if(cnt % 2 == 0) return -0.6; // 짝수인 경우
            else return 0.6; // 홀수인 경우
        }
    }
    else  {
        if(nextStone != 1 && remainning_stone[1] ==1) return 0;
        // 1번 돌을 아직 가지고 가지 않는 경우 0반환
        else if(nextStone == 1) { // 마지막에 1을 가지고 간 경우
            temp[1] = 0; 
            for(i=0; i<num_of_stones; i++)
                if(temp[i] != 0) cnt++;
            if (cnt % 2 == 0) return 0.5; // 짝수인 경우
            else return -0.5; // 홀수인 경우
        }
        else if(lastStone % nextStone == 0) {  // 마지막에 소수를 가지고 간 경우
            if((num_of_stones / nextStone) % 2 == 0) return 0.7; //짝수인 경우
            else return -0.7; // 홀수인 경우
        }
        else if(lastStone % nextStone != 0) { // 마지막에 소수가 아닌 돌을 가지고 간 경우
            cnt = 0;
            for(i=0; i<nextStone; i++)
                if(nextStone % i == 0) cnt++;
            if(cnt % 2 == 0) return 0.6; // 짝수인 경우
            else return -0.6; //홀수인 경우
        }
    }
}









