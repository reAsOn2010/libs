#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node/*store every party as a node in a tree*/
{
	int idx, deg, l, h;/*idx is the id of the party, the first is 1 and so on*/
};/*deg store the degree of joy*//*l store the length*//*h store the host*/
struct node s[35];/*store all the nodes*/
int n;/*the first parameter*/
int ans[35];/*the final answer of the parties to attend*/
int ret;/*store the sum of degrees*/
int use[35];/*store whether the party is attended*/
int maxsum;/*store the maximum sum of degrees*/

int cmp(const void *a, const void *b)/*to use the qsort*/
{
	return (*((struct node*)a)).h - (*((struct node*)b)).h;
}

void dfs(int id, int us, int timesum, int ddeg)/*depth first search*/
{
	int i;
	if(ret == maxsum) return;/*pruning: if a solution reach the maximun of the degree,return*/
	use[id] = us;/*the us actually a boolean, 1 represent the party is attended*/
	if(timesum > s[id].h) return;/*pruning: if attending the next party leads to overlapping the time limit, return*/
	if(id == n)/*if reached the bottom of the tree*/
	{
		if(ddeg > ret)/*if this solution's deg is large than former*/
		{
			ret = ddeg;/*update the answer*/
			for(i = 1; i <= n; i++)/*copy the solution to the answer array*/
			{
				ans[i] = use[i];/*copy*/
			}
		}
		return;
	}
	dfs(id+1, 1, timesum + s[id+1].l, ddeg + s[id+1].deg);/*recursively search the next one, this party is attended*/
	dfs(id+1, 0, timesum, ddeg);/*recursively search the next one, this party is not attended*/
}

int main(void)
{
	int i;
	int cnt;
	while(scanf("%d", &n))/*read the cases*/
	{
		if(n < 0) break;/*if n is negative, break out of the loop*/
		maxsum = 0;/*initialize the variables*/
		ret = 0;
		cnt = 0;
		memset(use, 0, sizeof(use));/*initialize the array*/
		for(i = 1; i <= n; i++)/*read the parties' information*/
		{
			scanf("%d%d%d", &s[i].deg, &s[i].l, &s[i].h);
			maxsum += s[i].deg;/*add all the degrees to the maxsum*/
			s[i].idx = i;/*store the party id*/
		}
		qsort(s+1, (size_t)n, (size_t)(sizeof(struct node)), cmp);/*sort the parties, by increasing order according to the parameter h*/
		dfs(1, 1, s[1].l, s[1].deg);/*if the first party is taken, begin dfs*/
		dfs(1, 0, 0, 0);/*if the first is not taken, begin dfs*/
		printf("%d\n", ret);/*print the maximum degree*/
		for(i = 0; i <= n; i++)/*print the party to attend*/
		{
			if(ans[i])
			{
				if(cnt) printf(" ");
				printf("%d", s[i].idx);
				cnt++;
			}
		}
		printf("\n");
	}
	return 0;
}

/*Pseudo-code:
 * 
 * depth_first_search(party_id, attend_or_not, time_sum, degree)
 * {
 *	if(this solution reach the maximum degree)
 *		return;
 *	if(the time after adding the next party is larger than time limit)
 *		return;
 *	if(all the parties have been checked)
 *	{
 *		if(this solution is better than before)
 *		{
 *			update the answer;
 *			update the party to attend;
 *		}
 *		return;
 *	}
 *	depth_first_search(next_party_id, attend, new_time_sum, new_degree);
 *	depth_first_search(next_party_id, not_attend, time_sum, degree);
 * }
 *
 * main()
 * {
 *	sort all the parties by increasing order according to the latest ending time;
 *	depth_first_search(first_party_id, attend, time, deg);
 *	depth_first_search(first_party_id, not_attend, 0, 0);
 *	print the answer
 * }
 */
