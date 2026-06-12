#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<bits/stdc++.h>
#include "../Lib/tlpi_hdr.h"
using namespace std;
void dfs(map<int,vector<int>>&g,map<int,string>&m,int u,int d) {
    for (int i=0;i<d-1;i++) printf("  ");
    printf("|");
    printf("__");
    cout<<m[u]<<endl;
    for(int v:g[u]) {
        dfs(g,m,v,d+1);
    }

}

int main(int argc,char *argv[]) {
	uid_t useruid=getuid();
	DIR *dir;
	struct dirent *entry;
	dir=opendir("/proc");
	if (dir==NULL) errExit("opendir");
	char uidstr[20];
	sprintf(uidstr,"%d",useruid);
	const int MAXPATH=100;
	const int MAXLENGTH=1024;
	char filename[MAXPATH];
	char line[MAXLENGTH];
	map<int,int> m;
	map<int,string> name;
	while((entry=readdir(dir))!=NULL) {
		if (!isdigit(entry->d_name[0])) continue;
		snprintf(filename,MAXPATH,"/proc/%s/status",entry->d_name);
		FILE *fp=fopen(filename,"r");
		if (fp==NULL) continue;
		bool ok=false;
		while(fgets(line,MAXLENGTH,fp)!=NULL) {
            if (strstr(line,"Name:")!=NULL) {
                char *token=strtok(line," \t");
                token=strtok(NULL," \t");
                if (token) {
                    int pid=atoi(entry->d_name);
                    name[pid]=string(token);
                }
            }
			if (strstr(line,"PPid:")!=NULL) {
				char *token=strtok(line," \t");
				token=strtok(NULL," \t");
				if (token) {
					int ppid=atoi(token);
					int pid=atoi(entry->d_name);
					m[pid]=ppid;

				}
				break;
			}
		}
	}
	for (auto &p: m) printf("[%d:%d]\n",p.first,p.second);
	for (auto &p: name) cout<<p.first<<":"<<p.second<<endl;
	map<int,vector<int>>g;
	for (auto &p:m) g[p.second].push_back(p.first);
	dfs(g,name,0,0);
}
