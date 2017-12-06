#include<cstdio>
#include<array>
using std::array;
#include<vector>
using std::vector;
#include<string>
using std::string;
const array<vector<int>,18>e{{
    {1,2,3},
    {4,5},
    {},
    {6,7},
    {8,9},
    {10,11},
    {12},
    {},
    {},
    {13,14},
    {15},
    {},
    {},
    {},
    {16,17},
    {},
    {},
    {},
}};
const array<int,18>v{
    -1,-1,14,-1,-1,
    -1,-1, 9,13,-1,
    -1, 4,10, 8,-1,
    17,13, 1,
};
int line_number = 0;
template<typename... Args>
void indent_printf(int n,Args... args){
    printf("%3d: ",line_number++);
    for(;n;--n){
        printf("|   ");
    }
    printf(args...);
}
int G2(int p,int alpha,int beta,int dep,string node_name);
int F2(int p,int alpha,int beta,int dep,string node_name){
    indent_printf(dep,"call procedure F2'(node=%8s,alpha=%3d,beta=%3d)\n",node_name.c_str(),alpha,beta);
    if(e[p].empty()){
        indent_printf(dep,"end F2'(node=%8s) as leaf node return %3d\n",node_name.c_str(),v[p]);
        return v[p];
    }
    int m = alpha;
    int i=0,n=e[p].size();
    for(;i<n;++i){
        int t = G2(e[p][i],m,beta,dep+1,node_name+'.'+char('1'+i));
        if(t > m){
            indent_printf(dep+1,"t = %3d >  m = %3d, m = %3d\n",t,m,t);
            m = t;
        }else{
            indent_printf(dep+1,"t = %3d <= m = %3d, no update on m\n",t,m);
        }
        if(m >= beta)break;
    }
    for(++i;i<n;++i){
        indent_printf(dep+1,"m = %3d >= beta = %3d, cut %8s\n",m,beta,(node_name+'.'+char('1'+i)).c_str());
    }
    indent_printf(dep,"end F2'(node=%8s) return %3d\n",node_name.c_str(),m);
    return m;
}
int G2(int p,int alpha,int beta,int dep,string node_name){
    indent_printf(dep,"call procedure G2'(node=%8s,alpha=%3d,beta=%3d)\n",node_name.c_str(),alpha,beta);
    if(e[p].empty()){
        indent_printf(dep,"end G2'(node=%8s) as leaf node return %3d\n",node_name.c_str(),v[p]);
        return v[p];
    }
    int m = beta;
    int i=0,n=e[p].size();
    for(;i<n;++i){
        int t = F2(e[p][i],alpha,m,dep+1,node_name+'.'+char('1'+i));
        if(t < m){
            indent_printf(dep+1,"t = %3d <  m = %3d, m = %3d\n",t,m,t);
            m = t;
        }else{
            indent_printf(dep+1,"t = %3d >= m = %3d, no update on m\n",t,m);
        }
        if(m <= alpha)break;
    }
    for(++i;i<n;++i){
        indent_printf(dep+1,"m = %3d <= alpha = %3d, cut %8s\n",m,alpha,(node_name+'.'+char('1'+i)).c_str());
    }
    indent_printf(dep,"end G2'(node=%8s) return %3d\n",node_name.c_str(),m);
    return m;
}
int main(){
    F2(0,-99,+99,0,"");
}

