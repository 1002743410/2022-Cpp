#include <iostream>

using namespace std;

//车
struct Car{
    int R;//所在车道
    int P;//车的位置
    int S;//速度
    int L;//最大速度
    int W;//变道策略
    bool isBlock= false;//是否阻塞
    bool isChangeLane;//是否可以发生变道
};

void swap(struct Car *a,struct  Car *b){
    struct Car swap={};
    swap=*a;
    *a=*b;
    *b=swap;
}

class ForzaHorizon{
public:
    ForzaHorizon(Car car[],int T,int N);
    void carSort();//车辆排序
    void laneUpdate();//车道状态
    void laneChange();//变道
    void accelerate();//加速
    void move();//移动

private:
    int N;//车辆数
    int T;//行驶时间
    Car *car;//车辆
};

ForzaHorizon::ForzaHorizon(Car *car, int T,int N) {
    this->car = car;
    this->T = T;
    this->N = N;
    carSort();
    //变道->加速->移动
    while (T > 0) {
        cout << T << ":" << endl;
        laneUpdate();
        laneChange();
        carSort();
        accelerate();
        move();
        T--;
        if (T > 0) {
            cout << T << ":" << endl;
            accelerate();
            move();
        }
        T--;
    }

    int min_P=0;//当前位置最小车辆标识
    int max_P=0;//当前车辆位置最大标识
    for (int i = 1; i < N; ++i) {
        if (car[min_P].P>car[i].P){
            min_P=i;
        }
        if (car[max_P].P<car[i].P){
            max_P=i;
        }
    }
    cout<<car[max_P].P-car[min_P].P;
}

void ForzaHorizon::carSort() {

    cout<<"beforesort:"<<endl;
    for (int i = 0; i < N; ++i) {
        cout<<car[i].R<<" "<<car[i].P<<" "<<car[i].S<<" "<<car[i].L<<" "<<car[i].W<<endl;
    }

    for (int i = 0; i < N;i++) {
        for (int j = 0; j < N-i; j++) {
            if (car[j].P<car[j + 1].P) {
                swap(&car[j],&car[j+1]);
            }
        }
    }

    cout<<"aftersort_P:"<<endl;
    for (int i = 0; i < N; ++i) {
        cout<<car[i].R<<" "<<car[i].P<<" "<<car[i].S<<" "<<car[i].L<<" "<<car[i].W<<endl;
    }


    for (int i = 0; i < N;i++) {
        for (int j = 0; j < N-i; j++) {
            if (car[j].R<car[j + 1].R) {
                swap(&car[j],&car[j+1]);
            }
        }
    }
    cout<<"aftersort_R:"<<endl;
    for (int i = 0; i < N; ++i) {
        cout<<car[i].R<<" "<<car[i].P<<" "<<car[i].S<<" "<<car[i].L<<" "<<car[i].W<<endl;
    }
}

void ForzaHorizon::laneUpdate() {

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if ((car[i].R==car[j].R+1 && car[i].P==car[j].P) || (car[i].R==car[j].R-1 && car[i].P==car[j].P)){
                car[i].isChangeLane= false;
            } else{
                if (j==N-1){
                    car[i].isChangeLane= true;
                }
            }
        }
    }

}

void ForzaHorizon::laneChange() {
    for (int i = N-1; i >0; i--) {
        if (car[i].isBlock && car[i].isChangeLane){
            //向车道序号小的车道变道
            if (car[i].W==2 && car[i].R!=1){
                car[i].R-=1;
            } else if (car[i].W==3 && car[i].R!=3){
                //向车道序号大的车道变道
                car[i].R+=1;
            } else{
                continue;
            }
        } else{
            continue;
        }
        car[i].isBlock= false;
    }
    cout<<"afterlanechange:"<<endl;
    for (int i = 0; i < N; ++i) {
        cout<<car[i].R<<" "<<car[i].P<<" "<<car[i].S<<" "<<car[i].L<<" "<<car[i].W<<endl;
    }
}

void ForzaHorizon::accelerate() {
    for (int i = 0; i < N; ++i) {
        if (car[i].isBlock){
            car[i].S=car[i-1].S;
        } else {
            if (car[i].S + 10 <= car[i].L) {
                car[i].S += 10;
            }
        }
    }
    cout<<"afteraccelerate:"<<endl;
    for (int i = 0; i < N; ++i) {
        cout<<car[i].R<<" "<<car[i].P<<" "<<car[i].S<<" "<<car[i].L<<" "<<car[i].W<<endl;
    }
}

void ForzaHorizon::move() {
    /*
     * 同一条车道内，车辆越靠前的车优先移动。
     * 以车辆当前速度的单位时间移动的距离作为单位距离。
     * 如果在车辆前方，单位距离的位置内或有位置上没有其他车辆，则向前移动一个单位距离。
     * 如果在车辆前方，单位距离的位置内或有位置上存在其他车辆，则向前移动到距离前车距离10的位置，且车辆的当前速度变为和前车的当前速度完全一样，并且进入阻塞状态。**/
    int distance[N];
    int temp;
    distance[0]=0;


    for (int i = 0; i < N;i++) {
        if (i==0){
            car[i].P += car[i].S;
        } else{
            temp = i - 1;
            if (car[i].R == car[temp].R) {
                distance[i]=car[temp].P-car[i].P;
                if (distance[i] > car[i].S) {
                    car[i].P += car[i].S;
                } else {
                    car[i].P = car[temp].P - 10;
                    car[i].S=car[temp].S;
                    car[i].isBlock = true;
                }
            } else {
                car[i].P += car[i].S;
            }
        }
    }

    cout<<"aftermove:"<<endl;
    for (int i = 0; i < N; ++i) {
        cout<<car[i].R<<" "<<car[i].P<<" "<<car[i].S<<" "<<car[i].L<<" "<<car[i].W<<endl;
    }
}


int main(){
    int T,N;//行驶时长，车辆数
    cin>>T>>N;
    Car car[N];
    for (int i = 0; i < N; ++i) {
        cin>>car[i].R>>car[i].P>>car[i].S>>car[i].L>>car[i].W;
    }
    ForzaHorizon(car,T,N);
}