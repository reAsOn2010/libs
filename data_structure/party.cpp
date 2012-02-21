#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
struct Node
{
	int idx,deg,l,h;
	void input()
	{
	scanf("%d%d%d",&deg,&l,&h);
	}
};
Node s[35];
int n;
bool ans[35];
int ret;
int maxsum;
bool use[35];
bool cmp(Node a,Node b)
{
	return a.h<b.h;
}
void dfs(int id,bool us,int timesum,int ddeg)
{
	if(ret==maxsum) return ;
	use[id]=us;
	if(timesum>s[id].h) return ;
	if(id==n)
	{
		if(ddeg>ret)
		{
			ret=ddeg;
			for(int i=1;i<=n;i++)
			{
				ans[i]=use[i];
			}
		}
	return ;
	}
	dfs(id+1,true,timesum+s[id+1].l,ddeg+s[id+1].deg);
	dfs(id+1,false,timesum,ddeg);
}
int main()
{
	while(scanf("%d",&n))
	{
		if(n<0) break;
		maxsum=0;
		memset(use,false,sizeof(use));
		for(int i=1;i<=n;i++)
		{
			s[i].input();
			maxsum+=s[i].deg;
			s[i].idx=i;
		}
		sort(s+1,s+1+n,cmp);
		ret=0;
		dfs(1,true,s[1].l,s[1].deg);
		dfs(1,false,0,0);
		printf("%d\n",ret);
		int cnt=0;
		for(int i=1;i<=n;i++)
		{
			if(ans[i])
			{
				if(cnt) printf(" ");
				printf("%d",s[i].idx);
				cnt++;
			}
		}
		printf("\n");
	}
	return 0;
} 
