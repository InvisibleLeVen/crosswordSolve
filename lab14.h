#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXWORDLENGTH 20

static int mapSize;

char **loadDict(const char *dict, int m)
{
    char **p = NULL;
    char buf[MAXWORDLENGTH] = {0};
    int counts = 0;

    FILE *readwords = fopen(dict, "r");
    if (!readwords)
    {
        printf("dictory file not found\n");
        return NULL;
    }
    p = malloc(m*sizeof(int*));
    while ((fgets(buf, MAXWORDLENGTH, readwords)) != NULL)
    {
        p[counts] = strdup(buf);
        counts++;
        if (counts == m)
            break;
    }
    fclose(readwords);

    for(int i = 0;i < m;i++){
        int len = strlen(p[i]);
        p[i][len-1] = '\0';
    }
    return p;
}

void freeDict(char **dict, int m)
{
    for (int i = 0; i < m; i++)
        free(dict[i]);
}

char *loadCorssWord(const char *map)
{
    char *p = NULL;
    FILE *readmap = fopen(map, "r");
    if (!readmap)
    {
        printf("dictory file not found\n");
        return NULL;
    }
    int cols = fgetc(readmap);
    cols -= '0';
    mapSize = cols;
    p = malloc(cols * cols);
    char c;
    int n = 0;

    while(1){
        c = fgetc(readmap);
        if(feof(readmap)){
            break;
        }
        if (isalpha(c))
            p[n++] = c;
        else if (c == '0')
            p[n++] = '0';
        else if (c == '*')
            p[n++] = '*';
    }
    fclose(readmap);
    return p;
}

void freeCrossWrod(char *map)
{
    free(map);
}

 struct line
{
    int x;
    int y;
    int length;
    bool dirct; // 0 is horizontal. 1 is vertical
};

struct line* getOneLine(char *map)
{
    struct line *res = NULL;
    // char *flag = malloc(mapSize * mapSize);
    // memset(flag, 0, mapSize * mapSize);
    bool dirct,hasline = 0;
    for (int i = 0; i < mapSize * mapSize; i++)
    {
        if (map[i] == '0')
        {
            if ((((i + 1) % mapSize != 0 && i != (mapSize * mapSize - 1)) && map[i + 1] != '*') || (((i % mapSize) != 0 && i != 0) && map[i - 1] != '*'))
            {
                int starti = i /mapSize*mapSize;
                for(int k = 0;k < mapSize;k++){
                    if(map[starti+k] == '0'){
                        hasline = 1;
                        break;
                    }
                }
                dirct = 0;

            }
            
            if(!hasline)
            {
                int starti = i % mapSize;
                for(int k = 0;k < mapSize;k++){
                    if(map[starti+k*mapSize] == '0'){
                        hasline = 1;
                        break;
                    }
                }
                dirct = 1;
            }

            if(!hasline){
                continue;
            }

            res = malloc(sizeof(struct line));
            if (dirct)
            {
                int tmp = i, length = 0;
                while (tmp >= mapSize && map[tmp-mapSize] != '*')
                    tmp -= mapSize;

                int tmp1 = tmp;
                while (tmp1 < mapSize * mapSize && map[tmp1] != '*')
                {
                    length++;
                    tmp1 += mapSize;
                }
                res->dirct = 1;
                res->length = length;
                res->x = tmp % mapSize;
                res->y = tmp / mapSize;
            }
            else
            {
                int tmp = i, length = 0;
                while (tmp % mapSize != 0 && map[tmp-1] != '*')
                    tmp -= 1;
                int tmp1 = tmp;
                while ((tmp1 % mapSize) != (mapSize - 1) && map[tmp1] != '*')
                {
                    length++;
                    tmp1 += 1;
                }

                res->dirct = 0;
                res->length = length;
                res->x = tmp % mapSize;
                res->y = tmp / mapSize;
            }
            break;
        }
    }
    // free(flag);
    return res;
}

bool ifFilled(char *map)
{
    for (int i = 0; i < mapSize * mapSize; i++)
    {
        if (map[i] == '0')
            return false;
    }
    return true;
}