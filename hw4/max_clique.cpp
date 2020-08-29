#include<cstdio>
#include<cstring>
#define N 1010

bool a[N][N];
bool group1[N];
int ans, cnt[N], group[N], n, m, vis[N];
bool dfs( int u, int pos )
{
    int i, j;
    for( i = u+1; i <= n; i++)
    {
        if( cnt[i]+pos <= ans ) return 0;
        if( a[u][i] )
        {
            for( j = 0; j < pos; j++ ) if( !a[i][ vis[j] ] ) break;
            if( j == pos )
            {
                vis[pos] = i;
                if( dfs( i, pos+1 ) ) return 1;
            }
        }
    }
    if( pos > ans )
    {
        for( i = 0; i < pos; i++ ){
            group[i] = vis[i];
        }

        ans = pos;
        return 1;
    }
    return 0;
}
void maxclique()
{
    ans=-1;
    for(int i=n;i>0;i--)
    {
        vis[0]=i;
        dfs(i,1);
        cnt[i]=ans;
    }
}
void init()
{
    memset(vis,0,sizeof vis);
    memset(group ,0,sizeof group);
    memset(group1 ,0,sizeof group1);
    memset(cnt,0,sizeof cnt);
}
int main()
{
    scanf("%d",&n);
    char ch;
    memset( a, 0, sizeof(a));
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++){
            scanf("%c",&ch);
            while(ch - '0' != 0 and ch - '0' != 1)
                scanf("%c",&ch);
            a[i][j] = ch - '0';
        }
    }
    init();
    maxclique();
    if( ans < 0 ) ans = 0;
    for(int i = 0; i < ans; i++){
        printf( i == 0 ? "%d" : " %d", group[i]);
        group1[group[i]] = true;
    }
    for(int i = 1; i <= n; i++){
        if(!group1[i]){
            printf(" %d", i);
        }
    }
    puts("");

}