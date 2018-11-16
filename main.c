#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct time {
    int hours;
    int mins;
    int secs;
    int msecs;
};

int getnum(FILE *,char temp);
void changer(struct time*,int,int);
int time_ret(struct time*);
void printer(struct time*);

FILE *primary , *target;
char temp;

int main(int argc,char **argv) {
char path[100];
int change,change_ms;
    if (argc >= 3) {
        primary = fopen(argv[1],"r");
        if (!primary) {
            printf("file given doesn't exist\n");
        }
        strcpy(path,argv[1]);
        change = atoi(argv[2]);
        if (change == 0) {
            printf("change number doesnt have any effect");
            return 0;
        }
        if (argv[3] == NULL)
            change_ms = 0;
        else
            change_ms = atoi(argv[3]);
    }
    else {
        printf("\nSpecify the unsyncronized subtitle file: ");
        scanf("%s",path);
        printf("\nSpesify time in seconds you wish the subtitle to be moved\n negative values are acceptable: ");
        scanf("%d",&change);
        printf("Now enter miliseconds: ");
        scanf("%d",&change_ms);
        primary = fopen(path,"r");
    }

    // File type confirmation
    
    char var[25];
    int limit,i,j = 0;
    limit = strlen(path);
    for (i = 0; i <= limit; i++) {
        temp = path[i];
        if (temp == '.')
            while (i <= limit) {
                temp = path[i];
                var[j] = temp;
                j++;
                i++;
            }
    }
    var[j] = '\0';

    if (strcmp(var,".srt") == 0) {
        printf("File valid\n");
        target = fopen("new_subs.srt","w");
    }
    else {
        printf("%s",var);
        printf("File is not supported... Yet\n");
        return 0;
    }
    // file check completed

    // Starting read-write sequence
    char num[10];
    int cur_line = 1,rnum;
    int variable;
    struct time temp_t;
    while (!feof(primary)) {
        fscanf(primary,"%c",&temp);
        if ((temp <= '9')&&(temp >= '0')) {
            rnum = getnum(primary,temp);
            if (rnum == cur_line) {
                cur_line++;
                fprintf(target,"%d",rnum);
                fscanf(primary,"%c",&temp);
                while (!((temp <= '9')&&(temp >= '0'))) {
                    fprintf(target,"%c",temp);
                    fscanf(primary,"%c",&temp);
                }
                fseek(primary,-sizeof(char),SEEK_SET);
                time_ret(&temp_t);
                changer(&temp_t,change,change_ms);
                printer(&temp_t);
                // Search for second segment
                fscanf(primary,"%c",&temp);
                while (!((temp >= '0')&&(temp <= '9'))) {
                    fprintf(target,"%c",temp);
                    fscanf(primary,"%c",&temp);
                }
                fseek(primary,-sizeof(char),SEEK_SET);
                time_ret(&temp_t);
                changer(&temp_t,change,change_ms);
                printer(&temp_t);
            }
        }
        else {
            fprintf(target,"%c",temp);
            }
        }
    fclose(primary);
    fclose(target);
    printf("\n Success \n");
    return 0;
}

int getnum(FILE *primary,char temp) {
    char num[10];
    int rnum,i;
    if (!temp) {
        fscanf(primary,"%c",&temp);
    }
    if ((temp >= '0')&&(temp <= '9')) {
        i = 0;
        while ((temp >= '0')&&(temp <= '9')) {
            num[i] = temp;
            fscanf(primary,"%c",&temp);
            i++;
        }
        num[i++] = '\0';
        fseek(primary,-1*sizeof(char),SEEK_CUR);
        return(atoi(num));
    }
    else {
        fseek(primary,-1*sizeof(char),SEEK_CUR);
        return temp;
    }
}

void changer(struct time *target,int change,int change_ms) {
    int total_s = target -> hours*3600 + target -> mins*60 + target -> secs;
    // Ms should be calculated first
    change_ms = target -> msecs + change_ms;
    total_s = total_s + change + (change_ms/1000);
    target -> msecs = change_ms % 1000;
    target -> hours = total_s / 3600;
    target -> mins = (total_s % 3600) / 60;
    target -> secs = (total_s % 3600) % 60;
}

int time_ret(struct time *result_str) {
    int i = 0,j,result[4];
    char temp, var[6];
    while (i < 4) {
        fscanf(primary,"%c",&temp);
        j = 0;
        var[0] = 'n';
        while((temp <= '9')&&(temp >= '0')) {
            var[j] = temp;
            fscanf(primary,"%c",&temp);
            j++;
        }
        if (var[0] != 'n') {
            var[j] = '\0';
            result[i] = atoi(var);
            i++;
        }
    }
    result_str -> hours = result[0];
    result_str -> mins = result[1];
    result_str -> secs = result[2];
    result_str -> msecs = result[3];
    fseek(primary,-sizeof(char),SEEK_CUR);
    return 0;
}

void printer(struct time *temp_t) {
    // printing sequence... oh god
    if (temp_t -> hours < 10) {
        fprintf(target,"0");
    } 
    fprintf(target,"%d:",temp_t -> hours);

    if (temp_t -> mins < 10) {
        fprintf(target,"0");
    }
    fprintf(target,"%d:",temp_t -> mins);

    if (temp_t -> secs < 10) {
        fprintf(target,"0");
    }
    fprintf(target,"%d,",temp_t -> secs);

    if (temp_t -> msecs < 100)
        fprintf(target,"0");
    if (temp_t -> msecs < 10)
        fprintf(target,"0");
    fprintf(target,"%d",temp_t -> msecs);
}