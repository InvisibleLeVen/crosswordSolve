#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lab14.h"

char* flagWords  = NULL;
int ansNum = 1;

bool FillOneLine(char* m,struct line* l,char* word){
    int starti = l->x+l->y*mapSize;
    if(l->dirct){
        for(int i = 0;i < strlen(word);i++){
            if(m[starti] != word[i] && m[starti] != '0')
                return 0;
            m[starti] = word[i];
            starti+=mapSize;
        }
    }else{
         for(int i = 0;i < strlen(word);i++){
            if(m[starti] != word[i] && m[starti] != '0')
                return 0;
            m[starti] = word[i];
            starti+=1;
        }
    }
    return 1;
}


void saveAns(char *map){
    struct stat st = {0};
    if(stat("./Solutions",&st) == -1){
        mkdir("Solutions",0700);
    }    
    char buf[10];
    sprintf(buf,"Solutions/%s%d.txt","sol",ansNum++);
    FILE *ansWrite = fopen(buf, "w");
    if (!ansWrite)
    {
        printf("can not open file\n");
        return;
    }
    for(int i = 0;i < mapSize*mapSize;i++){
        fputc(map[i],ansWrite);
        if((i+1)%mapSize == 0)
            fputc('\n',ansWrite);
        else
            fputc(' ',ansWrite);
    }
    fclose(ansWrite);
}

void solve(char *map,char** words,int nwords){
    if(ifFilled(map)){
        saveAns(map);
        return;
    }
    char* keep = malloc(mapSize*mapSize);
    memcpy(keep,map,mapSize*mapSize);
    struct line* lines = getOneLine(map);
    if(lines == NULL)
        return;
    for(int i = 0;i < nwords;i++){
        if(flagWords[i] == 1)
            continue;
        bool filled = FillOneLine(map,lines,words[i]);
        if(!filled)
            goto EndLoop;
        flagWords[i] = 1;
        solve(map,words,nwords);
        flagWords[i] = 0;
    EndLoop:
        memcpy(map,keep,mapSize*mapSize);
    }
    //memcpy(map,keep,mapSize*mapSize);
    free(keep);
}

int main(int argc,char** argv){
    char *dict = argv[1],*corssword = argv[2];
    int nans = atoi(argv[3]),nwords = atoi(argv[4]);//gets arguments

    char **loadedDict = loadDict(dict,nwords);
    char *map = loadCorssWord(corssword);
    flagWords = malloc(nwords);
    memset(flagWords,0,nwords);

    solve(map,loadedDict,nwords);

    freeDict(loadedDict,nwords);
    freeCrossWrod(map);
    free(flagWords);

    return 0;
}