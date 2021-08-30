#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{
    int fails = 0;
for (int a = 1; a<=4; a++){
    for (int b = 1; b<=4; b++){
        for (int c = 1; c<=4; c++){
            for (int d = 1; d<=4; d++){
                if (d==c){
                    fails++;
                    continue;
                }
                for (int e = 1; e<=4; e++){
                    if (e==c || e>=d-1){
                        fails++;
                        continue;
                    }
                    for (int f = 1; f<=4; f++){
                        if (abs(f-b) != 1 || c==f || d==f || abs(e-f)%2==0){
                            fails++;
                            continue;
                        }
                        for (int g = 1; g<=4; g++){
                            if (a<g || abs(g-c) != 1 || d<g || g==f){
                                fails++;
                                continue;
                            }
                            for (int h = 1; h<=4; h++){
                                if (a>=h || g>=h || abs(h-c) % 2 != 0 || h==d || e==h-2 || h==f){
                                    fails++;
                                    continue;
                                }
                                
                                    cout<<a<<b<<c<<d<<e<<f<<g<<h<<endl;
                                
                            }
                        }
                    }
                }
            }
        }
    }
}
cout<<fails<<endl;
return fails;
}
