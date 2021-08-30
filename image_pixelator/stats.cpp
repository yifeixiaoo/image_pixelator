
#include "stats.h"

stats::stats(PNG & im){
    xDim = im.width();
    yDim = im.height();
    sumHueX.resize(im.width());
    sumHueY.resize(im.width());
    sumSat.resize(im.width());
    sumLum.resize(im.width());
    hist.resize(im.width());
    for (unsigned int r = 0; r<im.width(); r++){
        sumHueX[r].resize(im.height());
        sumHueY[r].resize(im.height());
        sumSat[r].resize(im.height());
        sumLum[r].resize(im.height());
        hist[r].resize(im.height());
    }
    for (unsigned int c = 0; c<im.width();c++){
        for (unsigned int d = 0; d<im.height();d++){
            hist[c][d].resize(36);
        }
    }
    // hist.resize(im.width(), vector<vector<int>>(im.height(), vector<int>(36, 0)));
    


for(unsigned int i = 0; i<im.width();i++){
    for (unsigned int j = 0; j<im.height(); j++){
        HSLAPixel curr = *im.getPixel(i,j);
        if (i == 0 && j == 0){
            sumHueX[i][j] = cos((PI/180)*curr.h);
            sumHueY[i][j] = sin((PI/180)*curr.h);
            sumSat[i][j] = curr.s;
            sumLum[i][j] = curr.l;
            int x = (int) ((curr.h)/10);
            hist[i][j][x]++; 


        }else if (i == 0 && j != 0){
            sumHueX[i][j] = cos((PI/180)*curr.h) + sumHueX[i][j-1];
            sumHueY[i][j] = sin((PI/180)*curr.h) + sumHueY[i][j-1];
            sumSat[i][j] = curr.s + sumSat[i][j-1];
            sumLum[i][j] = curr.l + sumLum[i][j-1];
            int x = (int) ((curr.h)/10);
            hist[i][j] = hist[i][j-1];
            hist[i][j][x]++;



        }else if (i != 0 && j == 0){
            sumHueX[i][j] = cos((PI/180)*curr.h) + sumHueX[i-1][j];
            sumHueY[i][j] = sin((PI/180)*curr.h) + sumHueY[i-1][j];
            sumSat[i][j] = curr.s + sumSat[i-1][j];
            sumLum[i][j] = curr.l + sumLum[i-1][j];
            int x = (int) ((curr.h)/10);
            hist[i][j] = hist[i-1][j];
            hist[i][j][x]++;


        }else{
            sumHueX[i][j] = cos((PI/180)*curr.h) + sumHueX[i][j-1] + sumHueX[i-1][j] - sumHueX[i-1][j-1];
            sumHueY[i][j] = sin((PI/180)*curr.h) + sumHueY[i][j-1] + sumHueY[i-1][j] - sumHueY[i-1][j-1];
            sumSat[i][j] = curr.s + sumSat[i][j-1] + sumSat[i-1][j] - sumSat[i-1][j-1];
            sumLum[i][j] = curr.l + sumLum[i][j-1] + sumLum[i-1][j] - sumLum[i-1][j-1];
            int x = (int) ((curr.h)/10);
            hist[i][j] = hist[i-1][j];
            for (unsigned int n = 0; n<j; n++){
                HSLAPixel temp = *im.getPixel(i,n);
                int a = (int) ((temp.h)/10);
                hist[i][j][a]++;
            }
            hist[i][j][x]++;
            

    }

}


/* your code here */

}
    // for (int h=0;h<36;h++){
    //     cout<<hist[0][0][h]<<endl;
    // }
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    long x = (long) (lr.first - ul.first + 1);
    long y = (long ) (lr.second - ul.second + 1);
    return x * y;
/* your code here */

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    double total = rectArea(ul,lr);
    double avgH = 0;
    double avgS = 0;
    double avgL = 0;
    double avgX = 0;
    double avgY = 0;
    // double hue = sumHueY[lr.first][lr.second]/total,sumHueX[lr.first][lr.second]/total);
    if (ul.first == 0 && ul.second == 0){
        avgX = sumHueX[lr.first][lr.second];
        avgY = sumHueY[lr.first][lr.second];
        avgS = sumSat[lr.first][lr.second];
        avgL = sumLum[lr.first][lr.second];

    }else if (ul.first == 0 && ul.second != 0){
        avgX = sumHueX[lr.first][lr.second] - sumHueX[lr.first][ul.second-1];
        avgY = sumHueY[lr.first][lr.second] - sumHueY[lr.first][ul.second-1];
        avgS = sumSat[lr.first][lr.second] - sumSat[lr.first][ul.second-1];
        avgL = sumLum[lr.first][lr.second] - sumLum[lr.first][ul.second-1];

    }else if (ul.first != 0 && ul.second == 0){
        avgX = sumHueX[lr.first][lr.second] - sumHueX[ul.first-1][lr.second];
        avgY = sumHueY[lr.first][lr.second] - sumHueY[ul.first-1][lr.second];
        avgS = sumSat[lr.first][lr.second] - sumSat[ul.first-1][lr.second];
        avgL = sumLum[lr.first][lr.second] - sumLum[ul.first-1][lr.second];

    }else{
        avgX = sumHueX[lr.first][lr.second] - sumHueX[ul.first-1][lr.second] - sumHueX[lr.first][ul.second-1] + sumHueX[ul.first-1][ul.second-1];
        avgY = sumHueY[lr.first][lr.second] - sumHueY[ul.first-1][lr.second] - sumHueY[lr.first][ul.second-1] + sumHueY[ul.first-1][ul.second-1];
        avgS = sumSat[lr.first][lr.second] - sumSat[ul.first-1][lr.second] - sumSat[lr.first][ul.second-1] + sumSat[ul.first-1][ul.second-1];
        avgL = sumLum[lr.first][lr.second] - sumLum[ul.first-1][lr.second] - sumLum[lr.first][ul.second-1] + sumLum[ul.first-1][ul.second-1];
    }
    avgH = atan2(avgY,avgX)/(PI/180);
    if (avgH<0){
        avgH = avgH + 360;
    }
    avgS = avgS/total;
    avgL = avgL/total;
    return HSLAPixel(avgH,avgS,avgL,1.0);
/* your code here */
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
    vector<int> h;
    if (ul.first < 0 || ul.second < 0 || lr.first <0 || lr.second <0 || ul.first>(xDim-1) || ul.second>(yDim-1)||lr.first >(xDim-1) || lr.second>(yDim-1)){
        return vector<int> (36);
    }
    if (ul.first == 0 && ul.second == 0){

            for (int k = 0; k<36; k++){
                h.push_back(hist[lr.first][lr.second][k]);
            }


    }else if (ul.first == 0 && ul.second != 0){

            for (int k = 0; k<36; k++){
                h.push_back(hist[lr.first][lr.second][k] - hist[lr.first][ul.second-1][k]);
            }        


    }else if (ul.first != 0 && ul.second == 0){

            for (int k = 0; k<36; k++){
                h.push_back(hist[lr.first][lr.second][k] - hist[ul.first-1][lr.second][k]);
            }


    }else{
            for (int k = 0; k<36; k++){
                h.push_back(hist[lr.first][lr.second][k] - hist[ul.first-1][lr.second][k] - hist[lr.first][ul.second-1][k] + hist[ul.first-1][ul.second-1][k]);
            }


    }
    return h;


/* your code here */
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

/* your code here */

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){
    double area = rectArea(ul,lr);
    double entropy = 0.;
    vector<int> h = buildHist(ul,lr);

    for (int i = 0; i<36; i++){
        if (h[i] > 0) {
            entropy += ((double) h[i]/(double) area) * log2((double) h[i]/(double) area);
   
        }
            
            
    }
    return -1 * entropy;

/* your code here */

}
